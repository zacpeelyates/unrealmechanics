// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraPawn.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4PROJ_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//custom constructor
	ACustomPlayerController();
	ACameraPawn* CameraPawn;
	UCameraMovementActorComponent* CameraMovement;


protected:
	//overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//inputs
	virtual void SetupInputComponent() override;
	//movement axis
	virtual void DelegateMoveForward(float value);
	virtual void DelegateMoveStrafe(float value);
	//camera axis
	virtual void DelegateCameraZoom(float value);
	virtual void DelegateCameraPan(float value);
	virtual void DelegateCameraTilt(float value);
	virtual void DelegateCameraYaw(float value);
	virtual void DelegateCameraPitch(float value);
	virtual void DelegateCameraReset();
	
};





