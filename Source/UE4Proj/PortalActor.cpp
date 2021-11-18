// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalActor.h"
#include "kismet/GameplayStatics.h"
#include "PortalUtils.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsEnabled = false;
	
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	if (PortalPlaneMesh == nullptr)
	{
		
		TSet<UActorComponent*> Components =  GetComponents();
		//get portal plane mesh (where the portal material will be applied)
		for (UActorComponent* Comp : Components)
		{

			if (Comp->IsA(UStaticMeshComponent::StaticClass())) {
				UStaticMeshComponent* Mesh = (UStaticMeshComponent*)Comp;
				if (Mesh->GetStaticMesh()->GetName() == "Plane")
				{
					PortalPlaneMesh = Mesh;
				}
			}
			else if(Comp->IsA(USceneCaptureComponent2D::StaticClass()))
			{
				USceneCaptureComponent2D* SCC2D = (USceneCaptureComponent2D*)Comp;
				SceneCapture = SCC2D;
			}
		}
	}
	if(LinkedPortal == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Possible Missing Linked Portal!"))
	}

	
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PortalPlaneMesh->GetMaterial(0) != MaterialInstance) {
		PortalPlaneMesh->SetMaterial(0,MaterialInstance);
	}
}

bool APortalActor::IsEnabled()
{
	return bIsEnabled;
}

APortalActor* APortalActor::GetLinkedPortal()
{
	return LinkedPortal;
}

UTextureRenderTarget2D* APortalActor::GetRenderTexture()
{
	return RenderTexture;
}

UStaticMeshComponent* APortalActor::GetPortalPlaneMesh()
{
	return PortalPlaneMesh;
}

USceneCaptureComponent2D* APortalActor::GetSceneCaptureComponent()
{
	return SceneCapture;
}

void APortalActor::UpdateSceneCaptureRenderTarget()
{
	SceneCapture->TextureTarget = GetLinkedPortal()->GetRenderTexture();
	MaterialInstance->SetTextureParameterValue("RenderTextureParam",RenderTexture);
}

void APortalActor::SetEnabled(bool bIn)
{
	bIsEnabled = bIn;
}

void APortalActor::SetLinkedPortal(APortalActor* NewLinkedPortal)
{
	LinkedPortal = NewLinkedPortal;
}

void APortalActor::SetRenderTexture(UTextureRenderTarget2D* NewRenderTexture)
{
	RenderTexture = NewRenderTexture;

}

void APortalActor::SetMaterialInstance(UMaterialInstanceDynamic* NewMaterialInstanceDynamic)
{
	MaterialInstance = NewMaterialInstanceDynamic;
	PortalPlaneMesh->GetStaticMesh()->SetMaterial(0, MaterialInstance);
}


bool APortalActor::IsInPortal(FVector TargetLocation, FVector PortalLocation, FVector PortalNormal)
{
	//check values
	FVector IntersectionLocation;
	const FPlane PortalPlane = FPlane(PortalLocation, PortalNormal);
	const bool IsTargetInFrontOfPortal = PortalPlane.PlaneDot(TargetLocation) >= 0;
	const bool IsTargetIntersectingPortal = FMath::SegmentPlaneIntersection(LastTargetPosition, TargetLocation, PortalPlane, IntersectionLocation);
	//store values
	LastTargetPosition = TargetLocation;
	IsLastPositionInFrontOfPortal = IsTargetInFrontOfPortal;

	//target has passed through portal if:
	//its currently intersecting with portal
	//it was in front of the portal last check
	//it is now behind the portal (and needs to be teleported to the exit)
	return IsTargetIntersectingPortal && IsLastPositionInFrontOfPortal && !IsTargetInFrontOfPortal;
}


void APortalActor::TeleportActor(AActor* TargetActor)
{
	//store velocity
	FVector TeleportVelocity = TargetActor->GetVelocity();
	//location
	FVector NewLocation = PortalUtils::ConvertLocationToLocalSpace(TargetActor->GetActorLocation(), this, LinkedPortal);
	TargetActor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	//rotation
	TargetActor->SetActorRotation(PortalUtils::ConvertRotationToLocalSpace(TargetActor->GetActorRotation(), this, LinkedPortal));
	//reapply velocity in portal direction
	FVector OrientedVelocity;
	OrientedVelocity.X = FVector::DotProduct(TeleportVelocity, GetActorForwardVector());
	OrientedVelocity.Y = FVector::DotProduct(TeleportVelocity, GetActorRightVector());
	OrientedVelocity.Z = FVector::DotProduct(TeleportVelocity, GetActorUpVector());
	OrientedVelocity = OrientedVelocity.X * LinkedPortal->GetActorForwardVector()
		+ OrientedVelocity.Y * LinkedPortal->GetActorRightVector()
		+ OrientedVelocity.Z * LinkedPortal->GetActorUpVector();

	TargetActor->GetRootComponent()->ComponentVelocity = OrientedVelocity;
}

bool APortalActor::IsInBounds(FVector Location, UBoxComponent* Bounds)
{
	if (Bounds == nullptr) return false;

	const FVector Dir = Location - Bounds->GetComponentLocation();
	const FVector BoundsExtent = Bounds->GetScaledBoxExtent();

	return FMath::Abs(FVector::DotProduct(Dir, Bounds->GetForwardVector())) <= BoundsExtent.X
		&& FMath::Abs(FVector::DotProduct(Dir, Bounds->GetRightVector())) <= BoundsExtent.Y
		&& FMath::Abs(FVector::DotProduct(Dir, Bounds->GetUpVector())) <= BoundsExtent.Z;
}
