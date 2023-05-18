// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void URunAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Pawn && MovementComponent)
	{
		// Setting the current velocity and InAir status for the character
		bIsInAir = MovementComponent->IsFalling();
		FVector SpeedVector = MovementComponent->Velocity;
		SpeedVector.Z = 0.f;

		Speed = SpeedVector.Size();
	}
}
