// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalUtils.h"


FVector PortalUtils::ConvertLocationToLocalSpace(FVector Location, AActor* CurrentSpace, AActor* TargetSpace)
{
	if (CurrentSpace == nullptr || TargetSpace == nullptr)
	{
		return FVector::ZeroVector;
	}

	const FVector Direction = Location - CurrentSpace->GetActorLocation();
	FVector Dots;
	Dots.X = FVector::DotProduct(Direction, CurrentSpace->GetActorForwardVector());
	Dots.Y = FVector::DotProduct(Direction, CurrentSpace->GetActorRightVector());
	Dots.Z = FVector::DotProduct(Direction, CurrentSpace->GetActorUpVector());

	
	FVector NewDirection = Dots.X * TargetSpace->GetActorForwardVector()
	                           + Dots.Y * TargetSpace->GetActorRightVector()
		                       + Dots.Z * TargetSpace->GetActorUpVector();
	NewDirection.Y *= -1;
	return TargetSpace->GetActorLocation() + NewDirection;
}

FRotator PortalUtils::ConvertRotationToLocalSpace(FRotator Rotation, AActor* CurrentSpace, AActor* TargetSpace)
{
	if (CurrentSpace == nullptr || TargetSpace == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	FQuat TargetRotation = TargetSpace->GetTransform().GetRotation();
	FQuat InverseSpaceRotation = CurrentSpace->GetTransform().GetRotation().Inverse();
	FQuat RotQuat = FQuat(Rotation);
	FQuat FinalQuat = TargetRotation * InverseSpaceRotation * RotQuat;
	FRotator ReturnRot = FinalQuat.Rotator();
	return ReturnRot;
}
