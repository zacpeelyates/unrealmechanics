// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalUtils.h"


FVector PortalUtils::ConvertLocationToLocalSpace(FVector Location, AActor* CurrentSpace, AActor* TargetSpace)
{
	if (CurrentSpace == nullptr || TargetSpace == nullptr)
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

FRotator PortalUtils::ConvertRotationToLocalSpace(FRotator Rotation, AActor* CurrentSpace, AActor* TargetSpace)
{
	if (CurrentSpace == nullptr || TargetSpace == nullptr)
	{
		return FRotator::ZeroRotator;
	}
	return (TargetSpace->GetTransform().GetRotation() * CurrentSpace->GetTransform().GetRotation().Inverse() * FQuat(Rotation)).Rotator();
}
