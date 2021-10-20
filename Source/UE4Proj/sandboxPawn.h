// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "sandboxPawn.generated.h"

UCLASS()
class UE4PROJ_API AsandboxPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AsandboxPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input Functions
	void HandleMovementAxisX(float value);
	void HandleMovementAxisY(float value);
	void HandleJump();

	

};
