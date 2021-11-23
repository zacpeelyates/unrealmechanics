// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonDoor.h"

AButtonDoor::AButtonDoor()
{
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);
	//detatch triggerbox from rootcomponent and assign it to our button
	TriggerBox->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	TriggerBox->SetupAttachment(ButtonMesh);
}

void AButtonDoor::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->SetRelativeLocationAndRotation(FVector(0.0f),FRotator(0.0f));
	TriggerBox->InitBoxExtent(ButtonMesh->GetRelativeTransform().GetScale3D() * 1.05); //make trigger larger than button
}
