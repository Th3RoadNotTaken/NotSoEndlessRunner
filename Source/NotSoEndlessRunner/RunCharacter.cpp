// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NotSoEndlessRunnerGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainPlayerController.h"
#include "Obstacle.h"
#include "EndlessRunnerSaveGame.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	HighScores.Init(0.f, 3);
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<ANotSoEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);
	
	PlayerLocation = GetActorLocation();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	bGameCompleteted = false;

	if (MainPlayerController)
	{
		MainPlayerController->GameModeOnly();
	}
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Roll = 0.f;
	ControlRotation.Pitch = 0.f;

	UWorld* World = GetWorld();

	if (!bIsDead && !bGameCompleteted && World)
	{
		AddMovementInput(ControlRotation.Vector());
		
		if (bCanCountDistance)
		{
			bCanCountDistance = false;
			World->GetTimerManager().SetTimer(DistanceTravelTimerHandle, this, &ARunCharacter::AddDistance, 0.01, false);
		}

		if (bCanCountdown)
		{
			bCanCountdown = false;
			World->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ARunCharacter::ReduceTime, 1.f, false);
		}

		// Ensures that the sprint powerup is active and the timer is not already running before starting the powerup timer
		if (bSprintPowerupActive && !bTimerRunning)
		{
			bTimerRunning = true;
			World->GetTimerManager().SetTimer(SprintPowerupTimerHandle, this, &ARunCharacter::SprintPowerupCompleted, SprintPowerupDuration, false);
		}
	}
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ARunCharacter::BeginTouch);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ARunCharacter::EndTouch);
}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished()
{
	CurrentLane = NextLane;
}

void ARunCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if(!bIsDead && !bGameCompleteted)
		InitialTouchLocation = Location;
}

void ARunCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (bIsDead || bGameCompleteted)
	{
		return;
	}

	// Calculate the swipe direction
	FVector SwipeDirection = InitialTouchLocation - Location;

	if (SwipeDirection.Size() >= DesiredTouchDrag)
	{
		float XAbs = FMath::Abs(SwipeDirection.X);
		float YAbs = FMath::Abs(SwipeDirection.Y);

		if (XAbs > YAbs) // Left or Right Swipe
		{
			if (SwipeDirection.X > 0)
			{
				MoveLeft();
			}
			else
			{
				MoveRight();
			}
		}

		else // Up or Down Swipe
		{
			if (SwipeDirection.Y > 0)
			{
				SwipeUp();
			}
			else
			{
				SwipeDown();
			}
		}
	}
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::SwipeUp()
{
	ACharacter::Jump();
	if (JumpSound)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			UGameplayStatics::PlaySoundAtLocation(World, JumpSound, GetActorLocation());
		}
	}
}

void ARunCharacter::SwipeDown()
{
	static FVector ImpulseVector = FVector(0.f, 0.f, MoveDownImpulse);

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->AddImpulse(ImpulseVector, true);
	}
}

void ARunCharacter::Death()
{
	const FVector Location = GetActorLocation();
	UWorld* World = GetWorld();

	if (bIsDead)
		return;

	if (World)
	{
		bIsDead = true;
		if (DeathParticleSystem)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
		}
		if (DeathSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
		}
		GetMesh()->SetVisibility(false);

		World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 2.f);

		LoadGame();
		UpdateHighScores();
		SaveGame();
	}
}

void ARunCharacter::SprintPowerupUsed()
{
	if (bSprintPowerupActive)
		return;
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	if (MovementComp)
	{
		bSprintPowerupActive = true;
		MovementComp->MaxWalkSpeed = SprintPowerupMaxWalkSpeed;
	}
}

void ARunCharacter::SprintPowerupCompleted()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	UWorld* World = GetWorld();

	if (MovementComp && World && MainPlayerController && !bIsDead)
	{
		bSprintPowerupActive = false;
		bSprintPowerupCollected = false;
		MovementComp->MaxWalkSpeed = DefaultWalkSpeed;
		World->GetTimerManager().ClearTimer(SprintPowerupTimerHandle);
		bTimerRunning = false;
		MainPlayerController->RemoveSprintPowerupWidget();
	}
}

void ARunCharacter::TeleportPowerupUsed()
{
	if (MainPlayerController && !bIsDead)
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.X += TelelportDistance;
		SetActorLocation(CurrentLocation);
		bTeleportPowerupCollected = false;

		MainPlayerController->RemoveTeleportPowerupWidget();
	}
}

void ARunCharacter::OnDeath()
{

	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	if (MainPlayerController)
	{
		MainPlayerController->DisplayGameOverWidget();
	}
}

/** 
* HUD functions
*/

void ARunCharacter::AddDistance()
{
	FVector CurrentLocation = GetActorLocation();
	DistanceTravelled += (PlayerLocation - CurrentLocation).Size();
	PlayerLocation = CurrentLocation;
	bCanCountDistance = true;
}

void ARunCharacter::ReduceTime()
{
	bCanCountdown = true;
	if (TimeLeft == 0)
	{
		if (MainPlayerController)
		{
			bGameCompleteted = true;
			if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
			{
				MoveComp->MaxWalkSpeed = 0.f;
			}
			LoadGame();
			UpdateHighScores();
			SaveGame();
			MainPlayerController->DisplayGameOverWidget();
		}
		
	}
	else
	{
		TimeLeft--;
	}
}

// Update the Highscore table
void ARunCharacter::UpdateHighScores()
{
	float TravelledInM = DistanceTravelled / 100;
	if (TravelledInM <= HighScores[2])
		return;

	for (int i = 2; i >= 0; i--)
	{
		if (TravelledInM > HighScores[i])
		{
			if (i != 2)
				HighScores[i + 1] = HighScores[i];
			HighScores[i] = TravelledInM;

		}
		else
		{
			break;
		}
	}
}

void ARunCharacter::SaveGame()
{
	UEndlessRunnerSaveGame* SaveGameInstance = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UEndlessRunnerSaveGame::StaticClass()));

	for (int i = 0; i < 3; i++)
	{
		SaveGameInstance->HighScores[i] = HighScores[i];
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);
}

void ARunCharacter::LoadGame()
{
	UEndlessRunnerSaveGame* LoadGameInstance = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UEndlessRunnerSaveGame::StaticClass()));

	LoadGameInstance = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		for (int i = 0; i < 3; i++)
		{
			HighScores[i] = LoadGameInstance->HighScores[i];
		}
	}
}