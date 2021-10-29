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
	baseWalkSpeed = 5.0f;
	sprintFactor = 2;
	bIsSprint = false;
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
	//player axis
	InputComponent->BindAxis("Axis_Move_Forward", this, &ACustomPlayerController::DelegateMoveForward);
	InputComponent->BindAxis("Axis_Move_Strafe", this, &ACustomPlayerController::DelegateMoveStrafe);
	//camera axis
	InputComponent->BindAxis("Axis_Camera_Pan", this, &ACustomPlayerController::DelegateCameraPan);
	InputComponent->BindAxis("Axis_Camera_Tilt", this, &ACustomPlayerController::DelegateCameraTilt);
	InputComponent->BindAxis("Axis_Camera_Zoom", this, &ACustomPlayerController::DelegateCameraZoom);
	InputComponent->BindAxis("Axis_Camera_Pitch", this, &ACustomPlayerController::DelegateCameraPitch);
	InputComponent->BindAxis("Axis_Camera_Yaw", this, &ACustomPlayerController::DelegateCameraYaw);
	InputComponent->BindAxis("Axis_Camera_Roll", this, &ACustomPlayerController::DelegateCameraRoll);
	//actions
	//player actions
	InputComponent->BindAction("Action_Sprint", IE_Pressed, this, &ACustomPlayerController::SprintBegin);
	InputComponent->BindAction("Action_Sprint", IE_Released, this, &ACustomPlayerController::SprintEnd);
	//camera actions
	InputComponent->BindAction("Action_Camera_Reset", IE_Pressed, this, &ACustomPlayerController::DelegateCameraReset);
	InputComponent->BindAction("Action_Camera_FreeLook", IE_Pressed, this, &ACustomPlayerController::DelegateCameraFreeLookBegin);
	InputComponent->BindAction("Action_Camera_FreeLook", IE_Released, this, &ACustomPlayerController::DelegateCameraFreeLookEnd);
}

void ACustomPlayerController::SprintBegin()
{
	bIsSprint = true;
}

void ACustomPlayerController::SprintEnd()
{
	bIsSprint = false;
}

void ACustomPlayerController::DelegateCameraFreeLookBegin()
{
	CameraMovement->SetCameraFreeLook(true);
}

void ACustomPlayerController::DelegateCameraFreeLookEnd()
{
	CameraMovement->SetCameraFreeLook(false);
}

void ACustomPlayerController::DelegateCameraPan(float value)
{
	CameraMovement->MoveCamera(FVector(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateCameraTilt(float value)
{
	CameraMovement->MoveCamera(FVector(0.0f, value, 0.0f ));
}

void ACustomPlayerController::DelegateCameraZoom(float value)
{
	CameraMovement->MoveCamera(FVector(0.0, 0.0, value));
}

void ACustomPlayerController::DelegateCameraPitch(float value)
{
	CameraMovement->RotateCamera(FRotator(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateCameraYaw(float value)
{
	CameraMovement->RotateCamera(FRotator(0.0f, value, 0.0f));
}

void ACustomPlayerController::DelegateCameraRoll(float value)
{
	CameraMovement->RotateCamera(FRotator(0.0f, 0.0f, value));
}

void ACustomPlayerController::DelegateCameraReset()
{
	CameraMovement->ResetCameraLocation();
}

void ACustomPlayerController::DelegateMoveForward(float value)
{
	const float moveSpeed = bIsSprint ?  baseWalkSpeed * sprintFactor : baseWalkSpeed;
	CameraPawn->AddActorLocalOffset(FVector(value, 0, 0)*moveSpeed);
}

void ACustomPlayerController::DelegateMoveStrafe(float value)
{
	const float moveSpeed = bIsSprint ? baseWalkSpeed * sprintFactor : baseWalkSpeed;
	CameraPawn->AddActorLocalOffset(FVector(0, value, 0)*moveSpeed);
}

