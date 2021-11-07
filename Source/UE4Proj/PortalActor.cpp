// Fill out your copyright notice in the Description page of Project Settings.

#include "kismet/GameplayStatics.h"
#include "PortalActor.h"
#include "PortalUtils.h"

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
	
}

// Called every frame
void APortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APortalActor::IsEnabled()
{
	return bIsEnabled;
}

APortalActor* APortalActor::GetLinkedPortal()
{
	return LinkedPortal;
}

void APortalActor::SetEnabled(bool bIn)
{
	bIsEnabled = bIn;
}

void APortalActor::SetLinkedPortal(APortalActor* NewLinkedPortal)
{
	LinkedPortal = NewLinkedPortal;
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
	//reapply velocity in portal direction TODO: Move this to it's own method!
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

APortalManager* APortalActor::GetPortalManager()
{
	//this should be in a better place its very messy and breaks encapsulation but i just want it to work LOL
	return Cast<APortalManager>(UGameplayStatics::GetPlayerController(GetWorld(), 0))
}

