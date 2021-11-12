// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"

#include "EngineUtils.h"
#include "PortalActor.h"
#include "PortalUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
APortalManager::APortalManager()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PortalTexture = nullptr;
	ScreenX = 0;
	ScreenY = 0;

	//attach scene capture
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalSceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	//attach render 
	PortalTexture = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTexture"));
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AttachToActor(PlayerController, FAttachmentTransformRules::SnapToTargetIncludingScale);
	//get viewsize
	PlayerController->GetViewportSize(ScreenX, ScreenY);
	Initialize();
}

// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CreateRenderTarget();
	APortalActor* PortalToUpdate = UpdateClosestPortal();
	if (PortalToUpdate != nullptr)
	{
		UpdateView(PortalToUpdate);
	}
	
}

void APortalManager::HandleTeleport(APortalActor* PortalActor, AActor* TeleportActor)
{
	if(PortalActor != nullptr && TeleportActor != nullptr)
	{
		PortalActor->TeleportActor(TeleportActor);
		APortalActor* NewPortal = UpdateClosestPortal();
		if(NewPortal != nullptr)
		{
			UpdateView(NewPortal);
		}
	}
}

void APortalManager::Initialize()
{
	//create portal texture
	CreateRenderTarget();
	//setup scene capture
	SceneCapture->bCaptureEveryFrame = false;
	SceneCapture->bCaptureOnMovement = false;
	SceneCapture->bEnableClipPlane = true;
	SceneCapture->bUseCustomProjectionMatrix = true;
	SceneCapture->TextureTarget = nullptr;
	SceneCapture->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDRNoAlpha;

	//override scene capture vars
	FPostProcessSettings PostProcessSettings;
	PostProcessSettings.bOverride_MotionBlurAmount = true;
	PostProcessSettings.MotionBlurAmount = 0.0f;
	PostProcessSettings.bOverride_ScreenPercentage = true;
	PostProcessSettings.ScreenPercentage = 100.0f;
	SceneCapture->PostProcessSettings = PostProcessSettings;
}

void APortalManager::CreateRenderTarget()
{
		check(PortalTexture);
		PortalTexture->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA16f;
		PortalTexture->Filter = TextureFilter::TF_Bilinear;
		PortalTexture->SizeX = ScreenX;
		PortalTexture->SizeY = ScreenY;
		PortalTexture->ClearColor = FLinearColor::White;
		PortalTexture->bNeedsTwoCopies = false;
		PortalTexture->bAutoGenerateMips = false;
		PortalTexture->AddressX = TextureAddress::TA_Clamp;
		PortalTexture->AddressY = TextureAddress::TA_Clamp;
		PortalTexture->UpdateResource();                   
}

APortalActor* APortalManager::UpdateClosestPortal()
{
	//only update closest portal
	float	ClosestDistance = FLT_MAX;
	APortalActor* ClosestPortal = nullptr;
	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		APortalActor* CurrentPortal = *PortalIter;
		CurrentPortal->ClearRenderTexture();
		FVector fLocation = PlayerController->GetFocalLocation();
		FVector portalLocation = CurrentPortal->GetActorLocation();
		const float CurrentDistance = FMath::Abs(FVector::Dist(fLocation, portalLocation));
		if(CurrentDistance < ClosestDistance)
		{
			ClosestDistance = CurrentDistance;
			ClosestPortal = CurrentPortal;
		}
	}
	//return closest portal
	return ClosestPortal;
}

void APortalManager::UpdateView(APortalActor* PortalActor)
{
	//TODO: Split all this up into nice methods its really a lot to look at right now
	UCameraComponent* PlayerCamera = PlayerController->CameraPawn->GetCamera();
	APortalActor* LinkedPortal = PortalActor->GetLinkedPortal();
	//location
	SceneCapture->SetWorldLocation(PortalUtils::ConvertLocationToLocalSpace(PlayerCamera->GetComponentLocation(), PortalActor, LinkedPortal));
	//rotation
	FTransform CameraTransform = PlayerCamera->GetComponentTransform();
	FTransform PortalTransform = PortalActor->GetActorTransform();
	FTransform LinkedPortalTransform = LinkedPortal->GetActorTransform();
	FQuat LocalQuaternion = PortalTransform.GetRotation().Inverse() * CameraTransform.GetRotation();
	SceneCapture->SetWorldRotation(LinkedPortalTransform.GetRotation() * LocalQuaternion);
	//clip plane
	SceneCapture->ClipPlaneNormal = LinkedPortal->GetActorForwardVector();
	SceneCapture->ClipPlaneBase = LinkedPortal->GetActorLocation() + SceneCapture->ClipPlaneNormal * -1.5f; //avoid pixel border issues with offset
	//projection matrix
	FSceneViewProjectionData PlayerViewProjectionData;
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, EStereoscopicPass::eSSP_FULL, PlayerViewProjectionData);
	SceneCapture->CustomProjectionMatrix = PlayerViewProjectionData.ProjectionMatrix;
	//enable portal and add texture
	PortalActor->SetEnabled(true);
	LinkedPortal->SetEnabled(true);
	//fix unlinked portal (shouldn't ever be called as linked portals are set in editor)
	if(LinkedPortal->GetLinkedPortal() != PortalActor)
	{
		LinkedPortal->SetLinkedPortal(PortalActor);
	}
	//setup new view texture
	PortalActor->SetRenderTexture(PortalTexture);
	SceneCapture->TextureTarget = PortalTexture;
	//capture scene
	SceneCapture->CaptureScene();

}



