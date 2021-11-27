// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProxDoor.h"
#include "ButtonDoor.generated.h"

/**
 * 
 */
UCLASS()
class UE4PROJ_API AButtonDoor : public AProxDoor
{
	GENERATED_BODY()
public:
	AButtonDoor();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonMesh;
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
};

