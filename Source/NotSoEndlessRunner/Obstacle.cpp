// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "RunCharacter.h"

// Sets default values
AObstacle::AObstacle()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(SceneComponent);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentHit.AddDynamic(this, &AObstacle::OnObstacleHit);
}

void AObstacle::OnObstacleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

		if (RunCharacter)
		{
			RunCharacter->Death();
		}
	}
}

