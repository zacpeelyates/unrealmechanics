// Fill out your copyright notice in the Description page of Project Settings.

#include "PortalActor.h"
#include "kismet/GameplayStatics.h"
#include "PortalUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/Character.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	IsLastPositionInFrontOfPortal = false;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();
	if (PortalPlaneMesh == nullptr)
	{
		//get portal plane mesh (where the portal material will be applied)
		for (UActorComponent* Comp : this->GetComponents())
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
				SceneCapture = (USceneCaptureComponent2D*)Comp;
			}
			else if(Comp->IsA(UBoxComponent::StaticClass()))
			{
				PortalBounds = (UBoxComponent*)Comp;
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
}


bool APortalActor::IsInPortal(AActor* Target)
{
	FVector PortalToTarget = Target->GetActorLocation() - GetActorLocation();
	PortalToTarget.Normalize();
	if(FVector::DotProduct(GetActorForwardVector(), PortalToTarget) >= 0)
	{
		IsLastPositionInFrontOfPortal = true;
	}
	else
	{
		if(IsLastPositionInFrontOfPortal)
		{
			IsLastPositionInFrontOfPortal = false;
			LinkedPortal->IsLastPositionInFrontOfPortal = false;
			return true;
		}
	}
	return false;
	//target has passed through portal if:
	//its currently intersecting with portal
	//it was in front of the portal last check
	//it is now behind the portal (and needs to be teleported to the exit this frame)

}


void APortalActor::TeleportActor(AActor* TargetActor)
{
	TargetActor->SetActorLocation(PortalUtils::ConvertLocationToLocalSpace(TargetActor->GetActorLocation(), this, LinkedPortal));
	FRotator Rot =  PortalUtils::ConvertRotationToLocalSpace(TargetActor->GetActorRotation(), this, LinkedPortal);
	Rot.Yaw += 180.0f;
	TargetActor->SetActorRotation(Rot);
	if(TargetActor->IsA(ACharacter::StaticClass()))
	{
		Rot.Pitch = 0.0f;
		Rot.Roll = 0.0f;
		const ACharacter* c = static_cast<ACharacter*>(TargetActor);
		c->GetController()->SetControlRotation(Rot);
	}
	

	//reapply velocity in portal direction
	const FVector TeleportVelocity = TargetActor->GetVelocity();
	FVector OrientedVelocity;
	OrientedVelocity.X = FVector::DotProduct(TeleportVelocity, GetActorForwardVector());
	OrientedVelocity.Y = FVector::DotProduct(TeleportVelocity, GetActorRightVector());
	OrientedVelocity.Z = FVector::DotProduct(TeleportVelocity, GetActorUpVector());
	OrientedVelocity = OrientedVelocity.X * LinkedPortal->GetActorForwardVector()
		+ OrientedVelocity.Y * LinkedPortal->GetActorRightVector()
		+ OrientedVelocity.Z * LinkedPortal->GetActorUpVector();

	TargetActor->GetRootComponent()->ComponentVelocity = OrientedVelocity;
	LinkedPortal->IsLastPositionInFrontOfPortal = false;
	LinkedPortal->UpdateSceneCaptureRenderTarget();
}

bool APortalActor::IsInBounds(AActor* Target)
{
	UStaticMeshComponent* TargetMesh = (UStaticMeshComponent*)Target->GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (TargetMesh == nullptr) return false;
	FVector Bounds = PortalBounds->GetScaledBoxExtent();
	FVector MeshToPortal = (TargetMesh->GetComponentLocation() - PortalBounds->GetComponentLocation()).GetAbs();
	return (MeshToPortal.X <= Bounds.X) && (MeshToPortal.Y <= Bounds.Y) && (MeshToPortal.Z <= Bounds.Z);
}

AActor* APortalActor::GetTarget()
{

	float ClosestDistance = FLT_MAX;
	TArray<AActor*> Actors;
	PortalBounds->GetOverlappingActors(Actors);
	if (Actors.Num() == 0) return nullptr;
	AActor* Target = nullptr;
	for (AActor* Actor : Actors)
	{
		if (!Actor->IsA(StaticClass())) {
			const float CurrentDistance = FMath::Abs(FVector::Dist(GetActorTransform().GetLocation(), Actor->GetActorTransform().GetLocation()));
			if (CurrentDistance < ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				Target = Actor;
			}
		}
		
	}
	return Target;
}
