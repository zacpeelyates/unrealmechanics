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
	DoorParent->SetRelativeLocation(FVector(0));

}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	CloseTransform = HingeMeshComponent->GetRelativeTransform();
	PrimaryActorTick.bCanEverTick = true;
	bIsOpen = false;
	bIsTransitioning = false;
	Dir = 1;
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTransitioning)
	{
		const float T = 0.01f;
		const float LerpAlpha = 0.02f;
		HingeMeshComponent->SetRelativeRotation(FMath::Lerp(FQuat(HingeMeshComponent->GetRelativeRotation()), GoalTransform.GetRotation(), LerpAlpha));
		HingeMeshComponent->SetRelativeLocation(FMath::Lerp(HingeMeshComponent->GetRelativeLocation(),  GoalTransform.GetLocation(), LerpAlpha));
		HingeMeshComponent->SetRelativeScale3D(FMath::Lerp(HingeMeshComponent->GetRelativeScale3D(), GoalTransform.GetScale3D(), LerpAlpha));
		if(HingeMeshComponent->GetRelativeTransform().Equals(GoalTransform,T))
		{
			
			bIsTransitioning = false;
		}
	}

}

void ADoorActor::Open()
{
	GoalTransform = OpenTransform;
	GoalTransform.SetRotation(FQuat(GoalTransform.Rotator() * Dir));
	bIsOpen = true;
	bIsTransitioning = true;
}

void ADoorActor::Close()
{
	GoalTransform = CloseTransform;
	bIsOpen = false;
	bIsTransitioning = true;
}

void ADoorActor::SetDir(FVector OtherLocation)
{
	FVector OverlapToDoor = OtherLocation - GetActorLocation();
	OverlapToDoor.Normalize();
	Dir = FMath::Sign(FVector::DotProduct(OverlapToDoor,GetActorRightVector()));
}

