// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NotSoEndlessRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NOTSOENDLESSRUNNER_API ANotSoEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<class AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumInitialFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	/** Array for the Y values of the location of the 3 lanes */
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<float> LaneSwitchValues;

	/** Creates an initial few floors based on NumInitialFloorTiles*/
	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	/** Spawns floor tiles at the NextSpawnPoint. Also spawns obstacles based on the boolean input parameter bSpawnItems */
	UFUNCTION(BlueprintCallable)
	class AFloorTile* AddFloorTile(const bool bSpawnItems);

protected:

	virtual void BeginPlay() override;
	
};
