// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

UCLASS()
class NOTSOENDLESSRUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly)
	class ANotSoEndlessRunnerGameModeBase* RunGameMode;

	UFUNCTION()
	void TriggerOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Destroys obstacles, pickups and floor tiles that are no longer going to be used */
	UFUNCTION()
	void DestroyFloorTileObjects();

	UPROPERTY()
	FTimerHandle DestroyHandle;

	/** Spawns obstacles based on a random probability
	*	Lane: The lane on which the items are to spawn
	*	NumBigObstacles: Ensures that at no point will there be 3 big obstacles in a line 
	*/
	UFUNCTION()
	void SpawnLaneItem(class UArrowComponent* Lane, int32& NumBigObstacles);

	/** Spawns powerups based on a random probability 
	*	Lane: The lane on which the items are to spawn
	*/
	UFUNCTION()
	void SpawnLanePowerup(class UArrowComponent* Lane);

	/** Array to store the obstacles and pickups already spawned in a tile so that they can be deleted once the player reaches the end of that tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> ChildActors;

	/**
	* Probabilty of spawning obstacles or powerups on the floor tile
	*/
	float SmallObstacleProbability_Min = 0.5f;
	float SmallObstacleProbability_Max = 0.8f;
	float BigObstacleProbability_Min = 0.8f;
	float BigObstacleProbability_Max = 1.f;

	float SprintPowerupProbability_Min = 0.f;
	float SprintPowerupProbability_Max = 0.03f;
	float TeleportPowerupProbability_Min = 0.03f;
	float TeleportPowerupProbability_Max = 0.05f;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* FloorMesh;

	/** The point at which the next tile needs to be attached */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UArrowComponent* AttachPoint;

	/** Central points for all 3 lanes */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RightLane;

	/** A trigger box which spawns a floor tile once the player overlaps with it and also deletes the floor tile objects that are left behind this new tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* FloorTriggerBox;

	/**
	* TSubclasses for Obstacles and Pickups to be set in the BP
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<class AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<class AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<class ASprintPickup> SprintPickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<class ATeleportPickup> TeleportPickupClass;

	/** Get the attachment point's transform value */
	const FTransform& GetAttachTransform()const;

	/** Spawns obstacle items on each lane based on a random probability */
	UFUNCTION(BlueprintCallable)
	void SpawnItems();

	/** Checks if the classes for all TSubclassof obstacle objects are set */
	bool IsValidSpawn();

	/** Spawns powerup items on each lane based on a random probability */
	UFUNCTION(BlueprintCallable)
	void SpawnPowerups();

	/** Checks if the classes for all TSubclassof powerup objects are set */
	bool IsValidPowerup();
};
