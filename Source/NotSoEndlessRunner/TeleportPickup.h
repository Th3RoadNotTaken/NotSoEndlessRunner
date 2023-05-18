// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TeleportPickup.generated.h"

/**
 * 
 */
UCLASS()
class NOTSOENDLESSRUNNER_API ATeleportPickup : public APickup
{
	GENERATED_BODY()

public:

	ATeleportPickup();

protected:

	virtual void BeginPlay() override;
	
	virtual void SphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
