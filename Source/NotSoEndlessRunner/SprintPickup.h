// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "SprintPickup.generated.h"

/**
 * 
 */
UCLASS()
class NOTSOENDLESSRUNNER_API ASprintPickup : public APickup
{
	GENERATED_BODY()

public:

	ASprintPickup();

protected:

	virtual void BeginPlay() override;

	virtual void SphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;	
};
