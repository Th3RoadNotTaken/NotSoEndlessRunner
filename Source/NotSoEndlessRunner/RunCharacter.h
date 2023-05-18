// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunCharacter.generated.h"

UCLASS()
class NOTSOENDLESSRUNNER_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Declaring a spring arm and a camera to track the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	class ANotSoEndlessRunnerGameModeBase* RunGameMode;

public:

	UPROPERTY(VisibleInstanceOnly)
	class AMainPlayerController* MainPlayerController;

	/** Variables that track the lane value the player is in */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	// Sets default values for this character's properties
	ARunCharacter();

	/** Blueprint implementable event which is triggered when the player moves either left or right...
		... This event interpolates the player's location from their current lane to the lane they want to move to. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lane")
	void ChangeLane();

	/** Update function called to interp to the lane the player wants to move to */
	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneUpdate(float Value);

	/** Called once the timeline completes and sets the NextLane to the CurrentLane */
	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneFinished();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particles")
	class UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
	class USoundBase* JumpSound;

protected:
	
	virtual void BeginPlay() override;

	/** These functions update the NextLane the player wants to move to and triggers the ChangeLane event */
	UFUNCTION()
	void MoveLeft();
	UFUNCTION()
	void MoveRight();

	/** These functions are called when the player swipes up or down on the screen */
	void SwipeUp();
	void SwipeDown();
	/** Impulse with which the player will be pushed down */
	float MoveDownImpulse = -500.f;

	/** Function called in response to first touch on the screen. Sets the starting touch location */
	void BeginTouch(const ETouchIndex::Type FingerIndex, FVector Location);
	/** Function called in response to touch being released. Calculates the swipe direction */
	void EndTouch(const ETouchIndex::Type FingerIndex, FVector Location);
	float DesiredTouchDrag = 100.f;
	FVector InitialTouchLocation;

	/**
	* Sprint Powerup Pickup Utility
	*/

	/** This function is called when the character uses a sprint powerup. It will increase the movement speed of the character for the specified time */
	UFUNCTION(BlueprintCallable)
	void SprintPowerupUsed();
	/** This function will reset the movement speed and the Sprint Powerup properties*/
	void SprintPowerupCompleted();
	/** Boolean to make sure that a new timer is not set again if the SprintPowerupTimer is still running */
	bool bTimerRunning=false;
	/** Timer Handle for the duration of the activated sprint powerup */
	FTimerHandle SprintPowerupTimerHandle;
	float SprintPowerupDuration = 5.f;
	float SprintPowerupMaxWalkSpeed = 1700.f;
	float DefaultWalkSpeed = 1300.f;

	/**
	* Teleport Powerup Pickup Utility
	*/

	/** This function is called when the character uses a teleport powerup. It will teleport the player 10m forward */
	UFUNCTION(BlueprintCallable)
	void TeleportPowerupUsed();
	float TelelportDistance = 1000.f;

	/** Called after a certain delay to display the game over widget */
	UFUNCTION()
	void OnDeath();

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	UPROPERTY()
	bool bIsDead=false;
	UPROPERTY()
	bool bGameCompleteted = false;

	/** Adds the distance that the character has moved since the DistanceTravelTimerHandle timer started */
	UFUNCTION()
	void AddDistance();
	FVector PlayerLocation = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel")
	float DistanceTravelled=0.f;
	FTimerHandle DistanceTravelTimerHandle;
	/** Boolean which doesn't let distance be added until the DistanceTravelTimerHandle timer has completed */
	bool bCanCountDistance = true;

	/** An array of elements storing the 3 highest scores */
	TArray<float> HighScores;
	/** This function gets a particular score out of the 3 high scores */
	UFUNCTION(BlueprintCallable)
	float GetScore(int32 Rank) { return HighScores[Rank]; }
	/** Updates the HighScore table according to the current Score */
	void UpdateHighScores();

	/** Max time for the countdown */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Countdown")
	float MaxTime=180.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Countdown")
	float TimeLeft=180.f;
	FTimerHandle CountdownTimerHandle;
	bool bCanCountdown = true;
	/** Reduces the countdown timer. If 0, then it updates the highscore table and stops the character movement */
	UFUNCTION()
	void ReduceTime();

	/** 
	* Saving and loading the high scores
	*/

	void SaveGame();
	void LoadGame();

public:	

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Death();

	/** Boolean which ensures that the character cannot activate another sprint powerup while one is already active */
	bool bSprintPowerupActive = false;
	/** Ensures that another sprint powerup is not picked up if the player already has one in their inventory */
	bool bSprintPowerupCollected = false;

	/** Ensures that another teleport powerup is not picked up if the player already has one in their inventory */
	bool bTeleportPowerupCollected = false;
};
