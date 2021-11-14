// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ItemHolderComponent.h"
#include "CameraPawn.h"
#include "CustomPlayerController.generated.h"

//forward declare
class APortalManager;
UCLASS()
class UE4PROJ_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//custom constructor
	ACustomPlayerController();
	UPROPERTY(EditAnywhere)
	ACameraPawn* CameraPawn;
	UPROPERTY(EditAnywhere)
	UCameraMovementActorComponent* CameraMovement;
	UPROPERTY(EditAnywhere)
	UCharacterMovementComponent* PlayerMovement;
	UPROPERTY()
	UItemHolderComponent* ItemHolder;
	APortalManager* GetPortalManager();

protected:
	//protected vars
	bool bIsSprint;
	bool bIsFreelook;
	float BaseWalkSpeed;
	float SprintFactor;
	float MaxRoll;
	UPROPERTY(EditAnywhere)
	APortalManager* PortalManager;
	//overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//inputs
	virtual void SetupInputComponent() override;
	//movement axis
	virtual void DelegateMoveForward(float value);
	virtual void DelegateMoveStrafe(float value);
	//movement actions
	virtual void SprintBegin();
	virtual void SprintEnd();
	//camera axis
	virtual void DelegateCameraZoom(float value);
	virtual void DelegateCameraPan(float value);
	virtual void DelegateCameraTilt(float value);
	virtual void DelegateCameraYaw(float value);
	virtual void DelegateCameraPitch(float value);
	//camera actions
	virtual void DelegateCameraReset();
	virtual void DelegateCameraFreeLookBegin();
	virtual void DelegateCameraFreeLookEnd();
	virtual void DelegateCameraRollLeftBegin();
	virtual void DelegateCameraRollRightBegin();
	virtual void DelegateCameraRollLeftEnd();
	virtual void DelegateCameraRollRightEnd();
	virtual void DelegateToggleCamera();
	//item axis
	virtual void DelegateItemThrowForward(float value);
	virtual void DelegateItemThrowRight(float value);
	virtual void DelegateItemThrowUp(float value);
	//item actions
	virtual void DelegateItemPickup();
	virtual void DelegateItemRelease();
	virtual void DelegateItemThrow();

	
};





