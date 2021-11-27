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
		if (ClosestPortal->IsInPortal(Target) && ClosestPortal->IsInBounds(Target))
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
		FVector PortalLocation = CurrentPortal->GetActorLocation();
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
	APortalActor* LinkedPortal = Portal->GetLinkedPortal();
	if (LinkedPortal != nullptr)
	{
		const UCameraComponent* PlayerCam = PlayerCon->CameraPawn->GetActiveCamera();
		USceneCaptureComponent2D* LinkedSceneCapture = LinkedPortal->GetSceneCaptureComponent();

		FTransform ReversePortal = Portal->GetActorTransform();
		FRotator Rot = ReversePortal.Rotator();
		Rot.Yaw += 180;
		ReversePortal.SetRotation(FQuat(Rot));
		LinkedSceneCapture->SetRelativeTransform(UKismetMathLibrary::MakeRelativeTransform(PlayerCam->GetComponentTransform(),ReversePortal));

		LinkedSceneCapture->bOverride_CustomNearClippingPlane = true;
		LinkedSceneCapture->ClipPlaneNormal = LinkedPortal->GetActorForwardVector();
		LinkedSceneCapture->ClipPlaneBase = LinkedPortal->GetActorLocation();
		LinkedSceneCapture->CustomNearClippingPlane = FMath::Abs(FVector::Dist(PlayerCam->GetComponentLocation(), Portal->GetActorLocation()) - ClipBuffer);
		
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
		PortalTextureTarget->RenderTargetFormat = RTF_RGBA16f;
		PortalTextureTarget->Filter = TF_Bilinear;
		PortalTextureTarget->InitAutoFormat(ScreenX, ScreenY);
		CurrentPortal->SetMaterialInstance(PortalMaterialInstance);
		CurrentPortal->SetRenderTexture(PortalTextureTarget);
	}

	for (TActorIterator<APortalActor>PortalIter(GetWorld()); PortalIter; ++PortalIter)
	{
		(*PortalIter)->UpdateSceneCaptureRenderTarget();
	}
}




