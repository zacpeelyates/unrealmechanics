// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalManager.h"
#include "PortalUtils.h"
#include "EngineUtils.h"
#include "PortalActor.h"
#include "Components/SceneCaptureComponent2D.h"
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
	APortalActor* ClosestPortal = GetClosestPortal();
	UpdatePortalView(ClosestPortal);
	AActor* Target = ClosestPortal->GetTarget();
	if (Target != nullptr) {
		if (ClosestPortal->IsInBounds(Target) && ClosestPortal->IsInPortal(Target))
		{
			HandleTeleport(ClosestPortal, Target);
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
		FVector FocalLocation = PlayerCon->GetFocalLocation();
		FVector PortalLocation = CurrentPortal->GetActorLocation();
		const float CurrentDistance = FMath::Abs(FVector::Dist(FocalLocation, PortalLocation));
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
	APortalActor* LinkedPortal = Portal->GetLinkedPortal();
	if (LinkedPortal != nullptr)
	{
		UCameraComponent* PlayerCam = PlayerCon->CameraPawn->GetActiveCamera();
		USceneCaptureComponent2D* SceneCap = LinkedPortal->GetSceneCaptureComponent();
		//apply new location to scenecapture
		FVector NextCaptureLocation = PortalUtils::ConvertLocationToLocalSpace(PlayerCam->GetComponentLocation(), Portal, LinkedPortal);
		SceneCap->SetWorldLocation(NextCaptureLocation);

		FQuat LocalQuat = Portal->GetActorTransform().GetRotation().Inverse()* PlayerCam->GetComponentTransform().GetRotation();
		FQuat NextWorldQuat = LocalQuat * LinkedPortal->GetActorTransform().GetRotation();
		SceneCap->SetWorldRotation(NextWorldQuat);
		FVector CorrectedLocation = SceneCap->GetRelativeLocation();
		CorrectedLocation.X *= -1;
		FRotator CorrectedRotation = SceneCap->GetRelativeRotation();
		CorrectedRotation.Yaw += 180;
		PlayerCon->GetViewportSize(ScreenX, ScreenY);
		SceneCap->TextureTarget->ResizeTarget(ScreenX, ScreenY); //fixes textures breaking on window resize
		SceneCap->SetRelativeLocationAndRotation(CorrectedLocation, CorrectedRotation);

		SceneCap->bOverride_CustomNearClippingPlane = true;
		SceneCap->ClipPlaneNormal = LinkedPortal->GetActorForwardVector();
		SceneCap->ClipPlaneBase = LinkedPortal->GetActorLocation() * SceneCap->ClipPlaneNormal * -1.5f;
		SceneCap->CustomNearClippingPlane = FMath::Abs(FVector::Dist(PlayerCam->GetComponentLocation(), Portal->GetActorLocation()));
		
	}
}

void APortalManager::Init()
{
	//set up render textures for all portals in scene

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		APortalActor* CurrentPortal = *PortalIter;
		UMaterialInstanceDynamic* PortalMaterialInstance = UMaterialInstanceDynamic::Create(BasePortalMaterial, this);
		UTextureRenderTarget2D* PortalTextureTarget = NewObject<UTextureRenderTarget2D>();
		PortalTextureTarget->InitCustomFormat(ScreenX, ScreenY, PF_A16B16G16R16, true);
		CurrentPortal->SetMaterialInstance(PortalMaterialInstance);
		CurrentPortal->SetRenderTexture(PortalTextureTarget);
	}

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		(*PortalIter)->UpdateSceneCaptureRenderTarget();
	}
}




