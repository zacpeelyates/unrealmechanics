// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>((TEXT("MeshComponent")));	
	MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	CloseTransform = MeshComponent->GetRelativeTransform();
	PrimaryActorTick.bCanEverTick = true;
	bIsOpen = false;
	bIsTransitioning = false;
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTransitioning)
	{
		MeshComponent->SetRelativeRotation(FMath::Lerp(FQuat(MeshComponent->GetRelativeRotation()), GoalTransform.GetRotation(), LerpAlpha));
		MeshComponent->SetRelativeLocation(FMath::Lerp(MeshComponent->GetRelativeLocation(),  GoalTransform.GetLocation(), LerpAlpha));
	}
}
