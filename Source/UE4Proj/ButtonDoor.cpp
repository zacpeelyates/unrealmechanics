// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonDoor.h"

AButtonDoor::AButtonDoor()
{
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);
	ButtonMesh->bEditableWhenInherited = true; //not sure why other components let me edit them when inherited by default but this one doesn't
	//detatch triggerbox from rootcomponent and assign it to our button
	TriggerBox->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	TriggerBox->SetupAttachment(ButtonMesh);

}

// Called every frame
void AButtonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AButtonDoor::BeginPlay()
{
	Super::BeginPlay();
	Dir = 1; //not used in this class, set to 1 to ensure no unintended behaviour
}





 

