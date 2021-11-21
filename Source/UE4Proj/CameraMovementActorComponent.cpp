// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMovementActorComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UCameraMovementActorComponent::UCameraMovementActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bFirstPerson = false;

	//Set Defaults
	bCanCameraMove = true;
	DefaultCameraMovementSpeed = 5.0f;
	DefaultCameraRotationSpeed = 2.0f;
	CameraMoveFactor = 1.0f;
	MaxPitch = 80.0f;
	MinPitch = -80.0f;
}


// Called when the game starts
void UCameraMovementActorComponent::BeginPlay()
{
	Super::BeginPlay();
	CameraPawn = Cast<ACameraPawn>(GetOwner());
	PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FirstPersonCam = CameraPawn->GetFPCamera();
	ThirdPersonCam = CameraPawn->GetCamera();
}


// Called every frame
void UCameraMovementActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(FirstPersonCam == nullptr)
	{
		FirstPersonCam = CameraPawn->GetFPCamera();
	}
	if(ThirdPersonCam == nullptr)
	{
		ThirdPersonCam = CameraPawn->GetFPCamera();
	}
}


FVector UCameraMovementActorComponent::GetCameraForwardDirection()
{
	return CameraPawn->GetActorForwardVector();
}

FRotator UCameraMovementActorComponent::GetCameraRotation()
{
	return CameraPawn->GetActorRotation();
}

FVector UCameraMovementActorComponent::GetCameraLocation()
{
	return CameraPawn->GetActorLocation();
}

void UCameraMovementActorComponent::MoveCamera(FVector CameraDelta)
{
	if (!CameraDelta.IsZero() && bCanCameraMove) 
	{

		CameraDelta *= DefaultCameraMovementSpeed * CameraMoveFactor;
		ThirdPersonCam->AddRelativeLocation(CameraDelta, true);
	}
}


void UCameraMovementActorComponent::RotateCamera(FRotator CameraDelta)
{
	if (!CameraDelta.IsZero()) {
		if (!bFirstPerson)
		{
			if (bCanCameraMove)
			{
				CameraDelta *= DefaultCameraRotationSpeed * CameraMoveFactor;
				CameraPawn->AddArmRotation(CameraDelta);
			}
		}
		else
		{
			CameraDelta.Yaw = 0;
			CameraDelta.Roll = 0;
			//handle first person camera pitch (yaw is naturally controlled by rotation of player)
			float TotalPitch = CameraDelta.Pitch + FirstPersonCam->GetRelativeRotation().Pitch;
			if(TotalPitch < MaxPitch && TotalPitch > MinPitch)
			{
				FirstPersonCam->AddRelativeRotation(FQuat(CameraDelta));
			}
		}
	}
}


void UCameraMovementActorComponent::ResetCameraLocation(bool bKeepZoom)
{
	if (bCanCameraMove) {
		CameraPawn->SetCameraArmLengthToDefault(bKeepZoom);
	}
}


void UCameraMovementActorComponent::ToggleCamera()
{
	//wanted to do this using PlayerController SetViewTargetWithBlend but that wants the cameras to be in different objects
	//my character has two cameras attached to it so we have to do the less fancy method of turning things off and on again

	bFirstPerson = !bFirstPerson;
	if(bFirstPerson)
	{
		
		FirstPersonCam->Activate();
		ThirdPersonCam->Deactivate();
		
	}
	else
	{
		ThirdPersonCam->Activate();
		FirstPersonCam->Deactivate();
		FirstPersonCam->SetRelativeRotation(FRotator::ZeroRotator);
	}
	CameraPawn->GetStaticMesh()->ToggleVisibility();
}
