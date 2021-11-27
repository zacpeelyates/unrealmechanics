// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDoor.h"

void ADragDoor::BeginPlay()
{
	Super::BeginPlay();
	bIsInteracting = false;
}

void ADragDoor::InteractionBegin()
{
	bIsInteracting = true;
}

void ADragDoor::InteractionEnd()
{
	bIsInteracting = false;
	Close();
}

void ADragDoor::SetInteractor(AActor* Other)
{
	Interactor = Other;
}

void ADragDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsInteracting && Interactor != nullptr)
	{
		bIsTransitioning = true;
		float Dot = FVector::DotProduct(Interactor->GetActorForwardVector(), GetActorForwardVector()*-1) ;
		FTransform NewTransform;
		NewTransform.SetRotation(FQuat(OpenTransform.Rotator() * Dot));
		NewTransform.SetLocation(OpenTransform.GetLocation() * Dot);
		GoalTransform = NewTransform;
	}
}

