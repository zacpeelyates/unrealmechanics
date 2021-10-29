// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraPawn.h"
#include "Components/ActorComponent.h"
#include "CameraMovementActorComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJ_API UCameraMovementActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraMovementActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Can Camera move?
		bool bCanCameraMove;
		bool bIsCameraFreeLook;
	//Camera movement info
		float DefaultCameraMovementSpeed;
		float DefaultCameraRotationSpeed;
		float CameraMoveFactor;
		FVector CameraForwardDirection;

	//Camera rotation info
	float DeltaArm;
	FRotator DeltaRotation;
	float MinDistance;
	float MaxDistance;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Get Camera Pawn
	UPROPERTY(EditAnywhere)
		ACameraPawn* CameraPawn;
	//Get Player Controller
	UPROPERTY(EditAnywhere)
		APlayerController* PlayerController;


	UFUNCTION(Category = "Camera Direction")
		FVector GetCameraForwardDirection();
	UFUNCTION(Category = "Camera Rotation")
		FRotator GetCameraRotation();
	UFUNCTION(Category = "Camera Position")
		FVector GetCameraLocation();

	//Camera Controls
	UFUNCTION(Category = "Camera Movement")
		virtual void MoveCamera(FVector CameraDelta);

	UFUNCTION(Category = "Camera Movement")
		virtual void RotateCamera(FRotator CameraDelta);

	UFUNCTION(Category = "Camera Movement")
		virtual void ResetCameraLocation();

	UFUNCTION(Category = "Camera Movement")
		virtual void SetCameraFreeLook(bool bIn);





};
