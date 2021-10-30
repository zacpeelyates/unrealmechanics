// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

UCLASS()
class UE4PROJ_API APortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	AActor* VisualRepresentation;
	//getters
	bool IsEnabled();
	AActor* GetLinkedPortal();
	//setters
	void SetEnabled(bool bIn);
	void SetLinkedPortal(APortalActor* NewLinkedPortal);
	//portal functions
	bool IsInPortal(FVector TargetLocation, FVector PortalLocation, FVector PortalNormal);
	void TeleportActor(AActor* TeleportActor);

private:
	bool bIsEnabled;
	UPROPERTY(EditAnywhere);
	APortalActor* LinkedPortal;
	FVector LastTargetPosition;
	bool IsLastPositionInFrontOfPortal;
	//matrix transform utils
	FVector ConvertLocationToLocalSpace(FVector Location, AActor* CurrentSpace, AActor* TargetSpace);
	FRotator ConvertRotationToLocalSpace(FRotator Rotation, AActor* CurrentSpace, AActor* TargetSpace);
};