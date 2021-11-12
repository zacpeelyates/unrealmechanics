// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "CameraMovementActorComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ACameraPawn::ACameraPawn()
{
	//Set Default Values
	DefaultCameraZoom = 500.0f;
	MaxZoomDelta = DefaultCameraZoom / 2;
	DefaultCameraRotation = FRotator(-40.0f, 0.0f, 0.0f);
	//Set Root Component
	SetRootComponent(Cast<USceneComponent,UActorComponent>(GetComponentByClass(UCapsuleComponent::StaticClass())));
	RootCompRef = RootComponent;
	//Set Visual Player Mesh
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>((TEXT("MeshComponent")));
	MeshComponent->SetupAttachment(RootComponent);

	//Create + Attach Camera Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	//Set Camera Arm Defaults
	CameraArm->SetUsingAbsoluteRotation(false);
	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->bDoCollisionTest = false;
	CameraArm->bInheritPitch = false;

	//Create + Attach Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm);

	//Attach Item Holder Component
	ItemHolderComponent = CreateDefaultSubobject<UItemHolderComponent>(TEXT("ItemHolderComponent"));

	//Attach Camera Movement Component
	CameraMovementComponent = CreateDefaultSubobject<UCameraMovementActorComponent>(TEXT("CameraMovementComponent"));
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	if(!HasValidRootComponent())
	{
		SetRootComponent(RootCompRef); //i dont know why rootcomponent is sometimes null in runtime and im desperate
		//this actually worked help
	}
	SetCameraArmLengthToDefault(false);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Movement Definitions
void ACameraPawn::MoveCharacter(FVector MoveVec)
{
	RootComponent->AddLocalOffset(MoveVec);
}

void ACameraPawn::RotateCharacter(FRotator Rot)
{
	AddControllerPitchInput(Rot.Pitch);
	AddControllerYawInput(Rot.Yaw);
	AddControllerRollInput(Rot.Roll);
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
	CameraArm->TargetArmLength = FMath::Clamp(DeltaArmLength + CameraArm->TargetArmLength, DefaultCameraZoom-MaxZoomDelta,DefaultCameraZoom+MaxZoomDelta);
}

void ACameraPawn::AddArmRotation(FRotator DeltaArmRotation)
{
	const FRotator rotMax = FRotator(25.0f, 120.0f, 15.0f);
	FRotator rotNew = FRotator(CameraArm->GetRelativeRotation() + DeltaArmRotation);

	rotNew.Pitch = FMath::ClampAngle(rotNew.Pitch, DefaultCameraRotation.Pitch-rotMax.Pitch, DefaultCameraRotation.Pitch+rotMax.Pitch);
	rotNew.Yaw = FMath::ClampAngle(rotNew.Yaw, DefaultCameraRotation.Yaw-rotMax.Yaw, DefaultCameraRotation.Yaw+rotMax.Yaw);
	rotNew.Roll = FMath::ClampAngle(rotNew.Roll, DefaultCameraRotation.Roll-rotMax.Roll, DefaultCameraRotation.Roll+rotMax.Roll);

	CameraArm->SetRelativeRotation(rotNew);
}

void ACameraPawn::SetCameraArmLengthToDefault(bool bKeepZoom)
{
	if (!bKeepZoom)
	{
		CameraArm->TargetArmLength = DefaultCameraZoom;
	}
	CameraArm->SetRelativeRotation(DefaultCameraRotation);
}





