// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "ItemHolderComponent.h"
#include "CoreMinimal.h"
#include "CameraPawn.generated.h"

class UCameraMovementActorComponent;

UCLASS()
class UE4PROJ_API ACameraPawn : public ACharacter
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

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPCamera;


public:


	UPROPERTY(EditAnywhere)
	USceneComponent* RootCompRef;
	UPROPERTY(EditAnywhere)
	UCameraComponent* FPCameraRef;

	UPROPERTY(EditAnywhere)
		UItemHolderComponent* ItemHolderComponent;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Default CameraArm
	float DefaultCameraZoom;
	FRotator DefaultCameraRotation;
	UPROPERTY(EditAnywhere)
	UCameraMovementActorComponent* CameraMovementComponent;
	//Getter functions
	UFUNCTION(Category = "Getters")
		UCameraComponent* GetCamera() const { return PlayerCamera; }

	UFUNCTION(Category = "Getters")
		UCameraComponent* GetFPCamera() const { return FPCamera; }

	UFUNCTION(Category = "Getters")
		UCameraComponent* GetActiveCamera() const;

	UFUNCTION(Category = "Getters")
		USpringArmComponent* GetCameraArm() const { return CameraArm; }

	UFUNCTION(Category = "Getters")
		float GetCurrentCameraArmLength();
	UFUNCTION(Category = "Getters")
		FRotator GetCurrentCameraRotation();
	UFUNCTION(Category = "Getters")
		UStaticMeshComponent* GetStaticMesh();


	//Camera Arm Functions
	UFUNCTION()
		virtual void AddArmLength(float DeltaArmLength);
	UFUNCTION()
		virtual void AddArmRotation(FRotator DeltaArmRotation);
	UFUNCTION()
		virtual void SetCameraArmLengthToDefault(bool bKeepZoom = false);

	//Movement Functions
	void MoveCharacter(FVector MoveVec);
	void RotateCharacter(FRotator Rot);


};

