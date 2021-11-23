// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	DoorParent = CreateDefaultSubobject<USceneComponent>(TEXT("DoorComponent")); 
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>((TEXT("MeshComponent")));
	HingeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HingeComponent"));
	HingeMeshComponent->SetupAttachment(DoorParent);
	DoorMeshComponent->SetupAttachment(HingeMeshComponent);

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	CloseTransform = HingeMeshComponent->GetRelativeTransform();
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
		float T = 0.01f;
		HingeMeshComponent->SetRelativeRotation(FMath::Lerp(FQuat(HingeMeshComponent->GetRelativeRotation()), GoalTransform.GetRotation(), LerpAlpha));
		HingeMeshComponent->SetRelativeLocation(FMath::Lerp(HingeMeshComponent->GetRelativeLocation(),  GoalTransform.GetLocation(), LerpAlpha));
		HingeMeshComponent->SetRelativeScale3D(FMath::Lerp(HingeMeshComponent->GetRelativeScale3D(), GoalTransform.GetScale3D(), LerpAlpha));
		if(HingeMeshComponent->GetRelativeTransform().Equals(GoalTransform,T))
		{
			
			bIsTransitioning = false;
		}
	}

}

