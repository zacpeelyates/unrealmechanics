// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UE4PROJ_API PortalUtils
{
public:
	PortalUtils();
	~PortalUtils();

	static FVector ConvertLocationToLocalSpace(FVector Location, AActor* CurrentSpace, AActor* TargetSpace);
	static FRotator ConvertRotationToLocalSpace(FRotator Rotation, AActor* CurrentSpace, AActor* TargetSpace);
};
