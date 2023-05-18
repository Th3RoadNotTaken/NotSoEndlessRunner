// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPickup.h"
#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"

ATeleportPickup::ATeleportPickup()
{
}

void ATeleportPickup::BeginPlay()
{
	Super::BeginPlay();
}

void ATeleportPickup::SphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::SphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor)
	{
		ARunCharacter* RunChar = Cast<ARunCharacter>(OtherActor);
		if (RunChar && RunChar->MainPlayerController && !RunChar->bTeleportPowerupCollected)
		{
			RunChar->MainPlayerController->DisplayTeleportPowerupWidget();
			RunChar->bTeleportPowerupCollected = true;
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
