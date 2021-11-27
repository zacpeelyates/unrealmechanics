// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "CameraMovementActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "PortalManager.h"
#include "DragDoor.h"



ACustomPlayerController::ACustomPlayerController()
{
	//allow pawn to tick every frame
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bIsFreelook = false;
}

APortalManager* ACustomPlayerController::GetPortalManager()
{
	if(PortalManager == nullptr)
	{
		PortalManager = NewObject<APortalManager>();
	}
	return PortalManager;
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	BaseWalkSpeed = 5.0f;
	SprintFactor = 2;
	MaxRoll = 15.0f;
	bIsSprint = false;
	CameraPawn = Cast<ACameraPawn>(GetPawn());
	CameraMovement = CameraPawn->CameraMovementComponent;
	ItemHolder = CameraPawn->ItemHolderComponent;

}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//bindkeys
	check(InputComponent);
	//axis
	//player axis
	InputComponent->BindAxis("Axis_Move_Forward", this, &ACustomPlayerController::DelegateMoveForward);
	InputComponent->BindAxis("Axis_Move_Strafe", this, &ACustomPlayerController::DelegateMoveStrafe);
	//camera axis
	InputComponent->BindAxis("Axis_Camera_Pan", this, &ACustomPlayerController::DelegateCameraPan);
	InputComponent->BindAxis("Axis_Camera_Tilt", this, &ACustomPlayerController::DelegateCameraTilt);
	InputComponent->BindAxis("Axis_Camera_Zoom", this, &ACustomPlayerController::DelegateCameraZoom);
	InputComponent->BindAxis("Axis_Camera_Pitch", this, &ACustomPlayerController::DelegateCameraPitch);
	InputComponent->BindAxis("Axis_Camera_Yaw", this, &ACustomPlayerController::DelegateCameraYaw);
	//item axis
	InputComponent->BindAxis("Axis_Item_Throw_Forward", this, &ACustomPlayerController::DelegateItemThrowForward);
	InputComponent->BindAxis("Axis_Item_Throw_Right", this, &ACustomPlayerController::DelegateItemThrowRight);
	InputComponent->BindAxis("Axis_Item_Throw_Up", this, &ACustomPlayerController::DelegateItemThrowUp);
	//actions
	//player actions
	InputComponent->BindAction("Action_Sprint", IE_Pressed, this, &ACustomPlayerController::SprintBegin);
	InputComponent->BindAction("Action_Sprint", IE_Released, this, &ACustomPlayerController::SprintEnd);
	InputComponent->BindAction("Action_Jump", IE_Pressed, this, &ACustomPlayerController::JumpBegin);
	InputComponent->BindAction("Action_Jump", IE_Released, this, &ACustomPlayerController::JumpEnd);
	//camera actions
	InputComponent->BindAction("Action_Camera_Reset", IE_Pressed, this, &ACustomPlayerController::DelegateCameraReset);
	InputComponent->BindAction("Action_Camera_FreeLook", IE_Pressed, this, &ACustomPlayerController::DelegateCameraFreeLookBegin);
	InputComponent->BindAction("Action_Camera_FreeLook", IE_Released, this, &ACustomPlayerController::DelegateCameraFreeLookEnd);
	InputComponent->BindAction("Action_Camera_Roll_Left", IE_Pressed, this, &ACustomPlayerController::DelegateCameraRollLeftBegin);
	InputComponent->BindAction("Action_Camera_Roll_Left", IE_Released, this, &ACustomPlayerController::DelegateCameraRollLeftEnd);
	InputComponent->BindAction("Action_Camera_Roll_Right", IE_Pressed, this, &ACustomPlayerController::DelegateCameraRollRightBegin);
	InputComponent->BindAction("Action_Camera_Roll_Right", IE_Released, this, &ACustomPlayerController::DelegateCameraRollRightEnd);
	InputComponent->BindAction("Action_Camera_Toggle", IE_Pressed, this, &ACustomPlayerController::DelegateToggleCamera);
	//item actions
	InputComponent->BindAction("Action_Item_Pickup", IE_Pressed, this, &ACustomPlayerController::DelegateItemPickup);
	InputComponent->BindAction("Action_Item_Pickup", IE_Released, this, &ACustomPlayerController::DelegateItemRelease);
	InputComponent->BindAction("Action_Item_Throw", IE_Pressed, this, &ACustomPlayerController::DelegateItemThrow);
	//door actions
	InputComponent->BindAction("Action_Door_Interact", IE_Pressed, this, &ACustomPlayerController::DelegateDoorInteractionBegin);
	InputComponent->BindAction("Action_Door_Interact", IE_Released, this, &ACustomPlayerController::DelegateDoorInteractionEnd);
}

void ACustomPlayerController::SprintBegin()
{
	bIsSprint = true;
}

void ACustomPlayerController::SprintEnd()
{
	bIsSprint = false;
}

void ACustomPlayerController::JumpBegin()
{
	CameraPawn->Jump();
}

void ACustomPlayerController::JumpEnd()
{
	CameraPawn->StopJumping();
}

void ACustomPlayerController::DelegateCameraFreeLookBegin()
{
	bIsFreelook = true;
}

void ACustomPlayerController::DelegateCameraFreeLookEnd()
{
	bIsFreelook = false;
	CameraPawn->SetCameraArmLengthToDefault(true);
}

void ACustomPlayerController::DelegateCameraPan(float value)
{
	CameraMovement->MoveCamera(FVector(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateCameraTilt(float value)
{
	CameraMovement->MoveCamera(FVector(0.0f, value, 0.0f ));
}

void ACustomPlayerController::DelegateCameraZoom(float value)
{
	float zoomFactor = 50.0f;
	CameraPawn->AddArmLength(value*zoomFactor);
}

void ACustomPlayerController::DelegateCameraPitch(float value)
{
	
	CameraMovement->RotateCamera(FRotator(value, 0.0f, 0.0f));

}

void ACustomPlayerController::DelegateCameraYaw(float value)
{
	if (bIsFreelook)
	{
		CameraMovement->RotateCamera(FRotator(0.0f, value, 0.0f));
	}
	else
	{
		CameraPawn->RotateCharacter(FRotator(0, value, 0.0f));
	}
}


void ACustomPlayerController::DelegateCameraRollLeftBegin()
{
	CameraMovement->RotateCamera(FRotator(0.0f, 0.0f, -MaxRoll));
}

void ACustomPlayerController::DelegateCameraRollRightBegin()
{
	CameraMovement->RotateCamera(FRotator(0.0f, 0.0f, MaxRoll));
}

void ACustomPlayerController::DelegateCameraRollLeftEnd()
{
	CameraMovement->ResetCameraLocation(true);
}

void ACustomPlayerController::DelegateCameraRollRightEnd()
{
	CameraMovement->ResetCameraLocation(true);
}

void ACustomPlayerController::DelegateToggleCamera()
{
	CameraMovement->ToggleCamera();
}

void ACustomPlayerController::DelegateItemThrowForward(float value)
{
	ItemHolder->AddThrowVector(FVector(value, 0.0f, 0.0f));
}

void ACustomPlayerController::DelegateItemThrowRight(float value)
{
	ItemHolder->AddThrowVector(FVector(0.0f, value, 0.0f));
}

void ACustomPlayerController::DelegateItemThrowUp(float value)
{
	ItemHolder->AddThrowVector(FVector(0.0f, 0.0f, value));
}

void ACustomPlayerController::DelegateItemPickup()
{
	ItemHolder->RequestPickup();
}

void ACustomPlayerController::DelegateItemRelease()
{
	ItemHolder->RequestRelease();
}

void ACustomPlayerController::DelegateItemThrow()
{
	ItemHolder->RequestThrow();
}


AInteractDoor* Door;

void ACustomPlayerController::DelegateDoorInteractionBegin()
{
	const UBoxComponent* InteractionBox = ItemHolder->PickupBox; //this should not be done like this i need to move the box into the player class 
	TArray<AActor*> Actors;
	InteractionBox->GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		if(Actor->IsA(AInteractDoor::StaticClass()))
		{
			Door = Cast<AInteractDoor>(Actor);
			Door->SetDir(CameraPawn->GetActorLocation());
			Door->InteractionBegin();

			if(Door->IsA(ADragDoor::StaticClass()))
			{
				ADragDoor* DragDoor = Cast<ADragDoor>(Door);
				DragDoor->SetInteractor(CameraPawn);
			}
			break;
		}
	}
}

void ACustomPlayerController::DelegateDoorInteractionEnd()
{
	if(Door != nullptr)
	{
		Door->InteractionEnd();
	}
}


void ACustomPlayerController::DelegateCameraReset()
{
	CameraMovement->ResetCameraLocation();
}

void ACustomPlayerController::DelegateMoveForward(float value)
{
	const float moveSpeed = bIsSprint ?  BaseWalkSpeed * SprintFactor : BaseWalkSpeed;
	CameraPawn->MoveCharacter(FVector(moveSpeed * value, 0,0 ));
	
}

void ACustomPlayerController::DelegateMoveStrafe(float value)
{
	const float moveSpeed = bIsSprint ? BaseWalkSpeed * SprintFactor : BaseWalkSpeed;
	CameraPawn->MoveCharacter(FVector(0, moveSpeed*value, 0));
}

