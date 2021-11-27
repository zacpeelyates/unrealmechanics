// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractDoor.h"


void AInteractDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractDoor::InteractionBegin()
{
	if (bIsOpen)
	{
		Close();
	}
	else
	{
		Open();
	}
}

void AInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


