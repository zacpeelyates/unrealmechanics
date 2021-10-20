// Fill out your copyright notice in the Description page of Project Settings.


#include "sandboxPawn.h"

#include "Components/InputComponent.h"




// Sets default values
AsandboxPawn::AsandboxPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AsandboxPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AsandboxPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AsandboxPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Axis_Move_Forward", this, &AsandboxPawn::HandleMovementAxisY);
	InputComponent->BindAxis("Axis_Move_Strafe", this, &AsandboxPawn::HandleMovementAxisX);
	InputComponent->BindAction("Action_Jump", IE_Pressed, this, &AsandboxPawn::HandleJump);
}

void AsandboxPawn::HandleJump()
{
	
}


void AsandboxPawn::HandleMovementAxisY(float value)
{

}


void AsandboxPawn::HandleMovementAxisX(float value)
{
}

