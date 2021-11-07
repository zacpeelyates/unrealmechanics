// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraMovementActorComponent;

UCLASS()
class UE4PROJ_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//Player Camera
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamera;

	//CameraArm
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere)
		FRotator MaxRotationDelta;

	UPROPERTY(VisibleAnywhere)
		float MaxZoomDelta;

	//visual mesh representation
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Default CameraArm
	float DefaultCameraZoom;
	FRotator DefaultCameraRotation;
	UPROPERTY(EditAnywhere)
	UCameraMovementActorComponent* PawnMovementComponent;
	//Getter functions
	UFUNCTION(Category = "Getters")
		UCameraComponent* GetCamera() const { return PlayerCamera; }
	UFUNCTION(Category = "Getters")
		USpringArmComponent* GetCameraArm() const { return CameraArm; }

	UFUNCTION(Category = "Getters")
		float GetCurrentCameraArmLength();
	UFUNCTION(Category = "Getters")
		FRotator GetCurrentCameraRotation();

	//Camera Arm Functions
	UFUNCTION()
		virtual void AddArmLength(float DeltaArmLength);
	UFUNCTION()
		virtual void AddArmRotation(FRotator DeltaArmRotation);
	UFUNCTION()
		virtual void SetCameraArmLengthToDefault(bool bKeepZoom = false);


};
