// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintPickup.h"
#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"

ASprintPickup::ASprintPickup()
{
}

void ASprintPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ASprintPickup::SphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor)
	{
		ARunCharacter* RunChar = Cast<ARunCharacter>(OtherActor);
		if (RunChar && RunChar->MainPlayerController && !RunChar->bSprintPowerupCollected)
		{
			RunChar->MainPlayerController->DisplaySprintPowerupWidget();
			RunChar->bSprintPowerupCollected = true;
			if (PickupSound)
			{
				UWorld* World = GetWorld();
				if (World)
				{
					UGameplayStatics::PlaySoundAtLocation(World, PickupSound, GetActorLocation(), 1.4f);
					Destroy();
				}
			}
		}
	}
}
