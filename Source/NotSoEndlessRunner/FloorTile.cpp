// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NotSoEndlessRunnerGameModeBase.h"
#include "RunCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Obstacle.h"
#include "SprintPickup.h"
#include "TeleportPickup.h"

// Sets default values
AFloorTile::AFloorTile()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Trigger Box"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<ANotSoEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	check(RunGameMode);

	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::TriggerOnOverlapBegin);
}

const FTransform& AFloorTile::GetAttachTransform() const
{
	return AttachPoint->GetComponentTransform();
}

void AFloorTile::TriggerOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

		if (RunCharacter)
		{
			RunGameMode->AddFloorTile(true);

			GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTileObjects, 2.f, false);
		}
	}
}

void AFloorTile::DestroyFloorTileObjects()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	for (auto Child : ChildActors)
	{
		if (IsValid(Child))
		{
			Child->Destroy();
		}
	}

	ChildActors.Empty();
	this->Destroy();
}

void AFloorTile::SpawnLaneItem(UArrowComponent* Lane, int32& NumBigObstacles)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	UWorld* World = GetWorld();

	if (World)
	{
		if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SmallObstacleProbability_Min, SmallObstacleProbability_Max, true, false))
		{
			AObstacle* Obstacle = World->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
			ChildActors.Add(Obstacle);
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, BigObstacleProbability_Min, BigObstacleProbability_Max, true, true))
		{
			if (NumBigObstacles < 2) // Won't spawn 3 big obstacles in a line
			{
				AObstacle* Obstacle = World->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParameters);

				if (Obstacle)
				{
					NumBigObstacles += 1;
				}
				ChildActors.Add(Obstacle);
			}
			else
			{
				AObstacle* Obstacle = World->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
				ChildActors.Add(Obstacle);
			}
		}
	}
}

void AFloorTile::SpawnItems()
{
	if (IsValidSpawn())
	{
		int32 NumBigObstacles = 0;
		SpawnLaneItem(CenterLane, NumBigObstacles);
		SpawnLaneItem(LeftLane, NumBigObstacles);
		SpawnLaneItem(RightLane, NumBigObstacles);
	}
}

bool AFloorTile::IsValidSpawn()
{
	return (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass));
}

void AFloorTile::SpawnLanePowerup(UArrowComponent* Lane)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	FTransform SpawnLocation = Lane->GetComponentTransform();
	FVector SpawnVector = SpawnLocation.GetLocation();
	SpawnVector.Z+= 300.f;
	SpawnLocation.SetLocation(SpawnVector);

	if (UKismetMathLibrary::InRange_FloatFloat(RandVal, SprintPowerupProbability_Min, SprintPowerupProbability_Max, true, false))
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ASprintPickup* SprintPickup = World->SpawnActor<ASprintPickup>(SprintPickupClass, SpawnLocation, SpawnParameters);
			ChildActors.Add(SprintPickup);
		}
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, TeleportPowerupProbability_Min, TeleportPowerupProbability_Max, true, true))
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ATeleportPickup* TeleportPickup = World->SpawnActor<ATeleportPickup>(TeleportPickupClass, SpawnLocation, SpawnParameters);
			ChildActors.Add(TeleportPickup);
		}
	}
}

void AFloorTile::SpawnPowerups()
{
	if (IsValidPowerup())
	{
		SpawnLanePowerup(CenterLane);
		SpawnLanePowerup(LeftLane);
		SpawnLanePowerup(RightLane);
	}
}

bool AFloorTile::IsValidPowerup()
{
	return (IsValid(SprintPickupClass) && IsValid(TeleportPickupClass));
}
