// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

UCLASS()
class UE4PROJ_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Pickup(AActor* HoldActor);
	void Release();
	void Throw();
	void Preview();
	void SetThrowVector(FVector NewThrowVector);
	bool bCanCopy;
	bool bCanPickup;

private:
	UPROPERTY(EditAnywhere)
	FVector ThrowVector;
	UPROPERTY(EditAnywhere)
	UMaterial* PreviewMaterial;
	UPROPERTY(VisibleAnywhere)
	AActor* Holder; 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere)
	APickupActor* PreviewCopy;
	FVector PickupOffset;
	float BasePreviewTime;
	float PreviewTimer;
	ECollisionEnabled::Type DefaultCollisionType;
};
