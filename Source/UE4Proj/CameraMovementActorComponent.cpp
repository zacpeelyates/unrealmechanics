// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMovementActorComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UCameraMovementActorComponent::UCameraMovementActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	//Set Defaults
	bCanCameraMove = true;
	DefaultCameraMovementSpeed = 10.0f;
	CameraMoveFactor = 1.0f;
	MinDistance = 250.0f;
	MaxDistance = 750.0f;
}


// Called when the game starts
void UCameraMovementActorComponent::BeginPlay()
{
	Super::BeginPlay();
	CameraPawn = Cast<ACameraPawn>(GetOwner());
	PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}


// Called every frame
void UCameraMovementActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


FVector UCameraMovementActorComponent::GetCameraForwardDirection()
{
	return CameraPawn->GetActorForwardVector();
}

FRotator UCameraMovementActorComponent::GetCameraRotation()
{
	return CameraPawn->GetActorRotation();
}

FVector UCameraMovementActorComponent::GetCameraLocation()
{
	return CameraPawn->GetActorLocation();
}

void UCameraMovementActorComponent::MoveCamera(FVector CameraDelta)
{
	if(bCanCameraMove)
	{
		CameraDelta *= DefaultCameraMovementSpeed * CameraMoveFactor;
		CameraDelta.Z = FMath::Clamp(GetCameraLocation().Z + CameraDelta.Z, MinDistance, MaxDistance);
		CameraPawn->AddActorLocalOffset(CameraDelta,true);
	}
}

void UCameraMovementActorComponent::RotateCamera(FRotator CameraDelta)
{
	if (bCanCameraMove)
	{
		CameraDelta *= DefaultCameraMovementSpeed * CameraMoveFactor;
		CameraPawn->SetActorRotation(GetCameraRotation() + CameraDelta);
	}
}


void UCameraMovementActorComponent::ResetCameraLocation()
{
	CameraPawn->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	CameraPawn->SetCameraArmLengthToDefault();
}

