// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalActor.h"

// Sets default values
APortalActor::APortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsEnabled = false;
	if (VisualRepresentation != nullptr)
	{
		VisualRepresentation->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
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

AActor* APortalActor::GetLinkedPortal()
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
		FVector NewLocation = ConvertLocationToLocalSpace(TargetActor->GetActorLocation(), this, LinkedPortal);
		TargetActor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		//rotation
		TargetActor->SetActorRotation(ConvertRotationToLocalSpace(TargetActor->GetActorRotation(), this, LinkedPortal));
		//reapply velocity in portal direction TODO: Move this to it's own method!
		FVector OrientedVelocity;
		OrientedVelocity.X = FVector::DotProduct(TeleportVelocity, GetActorForwardVector());
		OrientedVelocity.Y = FVector::DotProduct(TeleportVelocity, GetActorRightVector());
		OrientedVelocity.Z = FVector::DotProduct(TeleportVelocity, GetActorUpVector());
		OrientedVelocity = OrientedVelocity.X * LinkedPortal->GetActorForwardVector()
			+ OrientedVelocity.Y * LinkedPortal->GetActorRightVector()
			+ OrientedVelocity.Z * LinkedPortal->GetActorUpVector();

		TargetActor->GetRootComponent()->ComponentVelocity = OrientedVelocity;
	
};

//utilty methods, could probably be in a static class somewhere
FVector APortalActor::ConvertLocationToLocalSpace(FVector Location, AActor* CurrentSpace, AActor* TargetSpace)
{
	if(CurrentSpace == nullptr || TargetSpace == nullptr)
	{
		return FVector::ZeroVector;
	}

	const FVector Direction = Location - CurrentSpace->GetActorLocation();
	FVector NewDirection;
	NewDirection.X = FVector::DotProduct(Direction, CurrentSpace->GetActorForwardVector());
	NewDirection.Y = FVector::DotProduct(Direction, CurrentSpace->GetActorRightVector());
	NewDirection.Z = FVector::DotProduct(Direction, CurrentSpace->GetActorUpVector());
	NewDirection = NewDirection.X * TargetSpace->GetActorForwardVector()
				 + NewDirection.Y * TargetSpace->GetActorRightVector()
				 + NewDirection.Z * TargetSpace->GetActorUpVector();
	return TargetSpace->GetActorLocation() + NewDirection;
}

FRotator APortalActor::ConvertRotationToLocalSpace(FRotator Rotation, AActor* CurrentSpace, AActor* TargetSpace)
{
	if (CurrentSpace == nullptr || TargetSpace == nullptr)
	{
		return FRotator::ZeroRotator;
	}
	return (TargetSpace->GetTransform().GetRotation() * CurrentSpace->GetTransform().GetRotation().Inverse() * FQuat(Rotation)).Rotator();
}


