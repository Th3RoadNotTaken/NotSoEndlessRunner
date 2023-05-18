// Copyright Epic Games, Inc. All Rights Reserved.


#include "NotSoEndlessRunnerGameModeBase.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/UserWidget.h"
#include "FloorTile.h"
#include "Kismet/GameplayStatics.h"

void ANotSoEndlessRunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialFloorTiles();
}

void ANotSoEndlessRunnerGameModeBase::CreateInitialFloorTiles()
{
	AFloorTile* Tile = AddFloorTile(false);
	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	AddFloorTile(false);
	AddFloorTile(false);

	for (int i = 0; i < NumInitialFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* ANotSoEndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();

	if (World)
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			if (bSpawnItems)
			{
				Tile->SpawnItems();
				Tile->SpawnPowerups();
			}

			NextSpawnPoint = Tile->GetAttachTransform();
		}

		return Tile;
	}
	return nullptr;
}
