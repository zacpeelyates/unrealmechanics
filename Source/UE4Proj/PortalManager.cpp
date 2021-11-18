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
	ScreenX = 1920;
	ScreenY = 1080;
}

// Called when the game starts or when spawned
void APortalManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AttachToActor(PlayerController, FAttachmentTransformRules::SnapToTargetIncludingScale);
	//get viewsize
	PlayerController->GetViewportSize(ScreenX, ScreenY);
	Init();
}



// Called every frame
void APortalManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APortalActor* PortalToUpdate = GetClosestPortal();
	
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

void APortalManager::Init()
{
	//set up render textures for all portals in scene

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		APortalActor* CurrentPortal = *PortalIter;
		UMaterialInstanceDynamic* PortalMaterialInstance = UMaterialInstanceDynamic::Create(BasePortalMaterial, this);
		UTextureRenderTarget2D* PortalTextureTarget = NewObject<UTextureRenderTarget2D>();
		PortalTextureTarget->InitCustomFormat(1920, 1080, PF_A16B16G16R16, true);
		CurrentPortal->SetMaterialInstance(PortalMaterialInstance);
		CurrentPortal->SetRenderTexture(PortalTextureTarget);
	}

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		(*PortalIter)->UpdateSceneCaptureRenderTarget();
	}
}





