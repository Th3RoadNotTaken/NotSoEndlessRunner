// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NOTSOENDLESSRUNNER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Reference to the DistanceTravelled UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> DistanceTravelledAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* DistanceTravelledWidget;

	/** Reference to the Countdown Timer UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> CountdownTimerAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* CountdownTimerWidget;

	/** Reference to the Sprint Powerup UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> SprintPowerupAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* SprintPowerupWidget;

	/** Reference to the Teleport Powerup UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> TeleportPowerupAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* TeleportPowerupWidget;

	/** Reference to the Game Over UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> GameOverAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets")
	UUserWidget* GameOverWidget;

protected:

	virtual void BeginPlay() override;

public:

	void DisplaySprintPowerupWidget();
	void RemoveSprintPowerupWidget();

	void DisplayTeleportPowerupWidget();
	void RemoveTeleportPowerupWidget();

	void DisplayGameOverWidget();

	void GameModeOnly();
	
};
