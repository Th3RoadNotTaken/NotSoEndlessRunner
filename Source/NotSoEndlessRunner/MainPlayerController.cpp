// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Application/SlateApplication.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DistanceTravelledAsset)
	{
		DistanceTravelledWidget = CreateWidget<UUserWidget>(this, DistanceTravelledAsset);
		if (DistanceTravelledWidget)
		{
			DistanceTravelledWidget->AddToViewport(0);
			DistanceTravelledWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (CountdownTimerAsset)
	{
		CountdownTimerWidget = CreateWidget<UUserWidget>(this, CountdownTimerAsset);
		if (CountdownTimerWidget)
		{
			CountdownTimerWidget->AddToViewport(0);
			CountdownTimerWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (SprintPowerupAsset)
	{
		SprintPowerupWidget = CreateWidget<UUserWidget>(this, SprintPowerupAsset);
		if (SprintPowerupWidget)
		{
			SprintPowerupWidget->AddToViewport(0);
			SprintPowerupWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (TeleportPowerupAsset)
	{
		TeleportPowerupWidget = CreateWidget<UUserWidget>(this, TeleportPowerupAsset);
		if (TeleportPowerupWidget)
		{
			TeleportPowerupWidget->AddToViewport(0);
			TeleportPowerupWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (GameOverAsset)
	{
		GameOverWidget = CreateWidget<UUserWidget>(this, GameOverAsset);
		if (GameOverWidget)
		{
			GameOverWidget->AddToViewport(100);
			GameOverWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::DisplaySprintPowerupWidget()
{
	if (SprintPowerupWidget)
	{
		SprintPowerupWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void AMainPlayerController::RemoveSprintPowerupWidget()
{
	if (SprintPowerupWidget)
	{
		SprintPowerupWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayTeleportPowerupWidget()
{
	if (TeleportPowerupWidget)
	{
		TeleportPowerupWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void AMainPlayerController::RemoveTeleportPowerupWidget()
{
	if (TeleportPowerupWidget)
	{
		TeleportPowerupWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayGameOverWidget()
{
	if (GameOverWidget)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
