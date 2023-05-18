// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EndlessRunnerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NOTSOENDLESSRUNNER_API UEndlessRunnerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UEndlessRunnerSaveGame();

	/** This will store the Player's highscores */
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<float> HighScores;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
};
