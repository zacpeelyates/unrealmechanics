// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "CameraMovementActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"



ACustomPlayerController::ACustomPlayerController()
{
	//allow pawn to tick every frame
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CameraPawn = Cast<ACameraPawn>(GetPawn());
	CameraMovement = CameraPawn->PawnMovementComponent;

}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//bindkeys
	check(InputComponent);
	//axis
	InputComponent->BindAxis("Axis_Move_Forward", this, &ACustomPlayerController::DelegateMoveForward);
	InputComponent->BindAxis("Axis_Move_Strafe", this, &ACustomPlayerController::DelegateMoveStrafe);
	InputComponent->BindAxis("Axis_Camera_Pan", this, &ACustomPlayerController::DelegateCameraPan);
	InputComponent->BindAxis("Axis_Camera_Tilt", this, &ACustomPlayerController::DelegateCameraTilt);
	InputComponent->BindAxis("Axis_Camera_Zoom", this, &ACustomPlayerController::DelegateCameraZoom);
	//actions
	InputComponent->BindAction("Action_Camera_Reset", IE_Pressed, this, &ACustomPlayerController::DelegateCameraReset);
}

void ACustomPlayerController::DelegateCameraPan(float value)
{
	CameraMovement->MoveCamera(FVector(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateCameraTilt(float value)
{
	CameraMovement->MoveCamera(FVector(0.0f, value, 0.0f));
}

void ACustomPlayerController::DelegateCameraPitch(float value)
{
	CameraMovement->RotateCamera(FRotator(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateCameraYaw(float value)
{
	CameraMovement->RotateCamera(FRotator(0.0f, value, 0.0f));
}


void ACustomPlayerController::DelegateCameraZoom(float value)
{
	CameraMovement->MoveCamera(FVector(0.0, 0.0, value));
}


void ACustomPlayerController::DelegateCameraReset()
{
	CameraMovement->ResetCameraLocation();
}


void ACustomPlayerController::DelegateMoveForward(float value)
{
	
}

void ACustomPlayerController::DelegateMoveStrafe(float value)
{
	
}

