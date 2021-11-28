// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "EngineUtils.h"
#include "PortalActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"




// Sets default values
APortalManager::APortalManager()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//default values in-case getviewportsize fails in init()
	ScreenX = 1920;
	ScreenY = 1080;
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
	//attach to playercontroller 
	PlayerCon = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AttachToActor(PlayerCon, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

bool hasInit = false;

// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!hasInit)
	{
		PlayerCon->GetViewportSize(ScreenX, ScreenY);
		Init();
		hasInit = true;
	}
	//Optimizations, fixed lag when there was a large amount of portals in the scene
	ClearRenderTextures(); //clear all portals 
	APortalActor* ClosestPortal = GetClosestPortal();
	if (ClosestPortal != nullptr) {
		UpdatePortalView(ClosestPortal); //activate closest portal and update the camera position for its render texdture

		//get closest teleport object to portal 
		AActor* Target = ClosestPortal->GetTarget();
		if (Target != nullptr) {
			//check target is in bounds and has crossed the portalplanemesh
			if (ClosestPortal->IsInPortal(Target) && ClosestPortal->IsInBounds(Target))
			{
				//teleport actor
				HandleTeleport(ClosestPortal, Target);
			}
		}
	}
}



void APortalManager::HandleTeleport(APortalActor* PortalActor, AActor* TeleportActor)
{
	if(PortalActor != nullptr && TeleportActor != nullptr)
	{
		PortalActor->TeleportActor(TeleportActor);
	}
	
}


APortalActor* APortalManager::GetClosestPortal()
{
	//only update closest portal
	float	ClosestDistance = FLT_MAX;
	APortalActor* ClosestPortal = nullptr;
	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		APortalActor* CurrentPortal = *PortalIter;
		FVector PortalLocation = CurrentPortal->GetActorLocation();
		//todo: find a clean way of not checking portals that are not on screen (recursive rendering through portals?)
		//tried dotproduct of camera's angle and portal position but it breaks when looking at the portal from weird angles/positions
		const float CurrentDistance = FMath::Abs(FVector::Dist(PlayerCon->CameraPawn->GetActorLocation(), PortalLocation));
		if(CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestPortal = CurrentPortal;
		}
	}
	//return closest portal
	return ClosestPortal;
}

void APortalManager::UpdatePortalView(APortalActor* Portal)
{
	Portal->GetSceneCaptureComponent()->Activate();
	APortalActor* LinkedPortal = Portal->GetLinkedPortal();
	if (LinkedPortal != nullptr)
	{
		
		const UCameraComponent* PlayerCam = PlayerCon->CameraPawn->GetActiveCamera();
		USceneCaptureComponent2D* LinkedSceneCapture = LinkedPortal->GetSceneCaptureComponent();
		LinkedSceneCapture->Activate();

		//get back-facing transform of the portal-to-enter
		FTransform ReversePortal = Portal->GetActorTransform();
		FRotator Rot = ReversePortal.Rotator();
		Rot.Yaw += 180;
		ReversePortal.SetRotation(FQuat(Rot));
		//set linkedportal's scene capture component transform relative to linkedportal to be the same as the transform between the active camera and the back of the portal-to-enter
		//this makes the portal effect work in 3d, instead of just keeping the camera at the exit of the portal, which would make the effect look flat  
		LinkedSceneCapture->SetRelativeTransform(UKismetMathLibrary::MakeRelativeTransform(PlayerCam->GetComponentTransform(),ReversePortal));
		//having the camera be behind the portal, however, means we need to move it's clipping plane forward to make sure it isn't rendering objects between it and it's portal,
		//we dont want to player to see those as it breaks the effect.
		//these numbers probably need tweaking still but it's close to where i want it to be 
		LinkedSceneCapture->ClipPlaneNormal = LinkedPortal->GetActorForwardVector();
		LinkedSceneCapture->ClipPlaneBase = LinkedPortal->GetActorLocation() - (LinkedSceneCapture->ClipPlaneNormal * ClipBuffer);
		
	}
}

void APortalManager::Init()
{
	//set up render textures for all portals in scene

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		APortalActor* CurrentPortal = *PortalIter;
		//create and set the material and associated render target for this portal 
		UMaterialInstanceDynamic* PortalMaterialInstance = UMaterialInstanceDynamic::Create(BasePortalMaterial, this);
		UTextureRenderTarget2D* PortalTextureTarget = NewObject<UTextureRenderTarget2D>();
		PortalTextureTarget->RenderTargetFormat = RTF_RGBA16f;
		PortalTextureTarget->Filter = TF_Bilinear;
		PortalTextureTarget->InitAutoFormat(ScreenX, ScreenY);
		CurrentPortal->SetMaterialInstance(PortalMaterialInstance);
		CurrentPortal->SetRenderTexture(PortalTextureTarget);
	}

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		//loop through again updating the render targets to each portals LinkedPortal's scenecapturecomponent (cant do in first loop as linkedportals may not be set up yet)
		(*PortalIter)->UpdateSceneCaptureRenderTarget();
	}
}

void APortalManager::ClearRenderTextures()
{
	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		//turn off scenecapture components when not in use to prevent lag 
		(*PortalIter)->GetSceneCaptureComponent()->Deactivate();
		(*PortalIter)->GetRenderTexture()->UpdateResourceImmediate(true);
	}
}




