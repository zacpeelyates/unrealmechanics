// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "CameraMovementActorComponent.h"


// Sets default values
ACameraPawn::ACameraPawn()
{
	//Set Default Values
	DefaultCameraZoom = 500.0f;
	DefaultCameraRotation = FRotator(-45.0f, 0.0f, 0.0f);
	//Set Root Component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	const float CollisionBoxSize = 30.0f;
	SetRootComponent(CollisionBox);
	CollisionBox->InitBoxExtent(FVector(CollisionBoxSize));

	//Create + Attach Camera Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//Set Camera Arm Defaults
	CameraArm->SetUsingAbsoluteRotation(false);
	CameraArm->SetRelativeRotation(DefaultCameraRotation);
	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->bInheritPitch = false;

	//Create + Attach Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm);

	//Attach Movement Component
	PawnMovementComponent = CreateDefaultSubobject<UCameraMovementActorComponent>(TEXT("CameraMovementComponent"));
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Camera Function Definitions
float ACameraPawn::GetCurrentCameraArmLength()
{
	return CameraArm->TargetArmLength;
}


FRotator ACameraPawn::GetCurrentCameraRotation()
{
	return CameraArm->GetRelativeRotation();
}


void ACameraPawn::AddArmLength(float DeltaArmLength)
{
	CameraArm->TargetArmLength += DeltaArmLength;
}

void ACameraPawn::AddArmRotation(FRotator DeltaArmRotation)
{
	const FRotator rotMax = FRotator(-25.0f, 0.0f, 0.0f);
	FRotator rotNew = FRotator(CameraArm->GetRelativeRotation() + DeltaArmRotation);

	rotNew.Pitch = FMath::ClampAngle(rotNew.Pitch, DefaultCameraRotation.Pitch, rotMax.Pitch);
	rotNew.Yaw = FMath::ClampAngle(rotNew.Yaw, DefaultCameraRotation.Yaw, rotMax.Yaw);
	rotNew.Roll = FMath::ClampAngle(rotNew.Roll, DefaultCameraRotation.Roll, rotMax.Roll);

	CameraArm->SetRelativeRotation(rotNew);
}

void ACameraPawn::SetCameraArmLengthToDefault()
{
	CameraArm->TargetArmLength = DefaultCameraZoom;
	CameraArm->SetRelativeRotation(DefaultCameraRotation);
}





