// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class NOTSOENDLESSRUNNER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	

	APickup();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USoundBase* PickupSound;

	/** Boolean to check if a pickup should keep rotating in the game */
	bool bShouldRotate = true;
	float RotationRate = 90.f;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
