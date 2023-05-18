// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerSaveGame.h"

UEndlessRunnerSaveGame::UEndlessRunnerSaveGame()
{
	HighScores.Init(0.f, 3);
	UserIndex = 0;
	PlayerName = TEXT("Default");
}
