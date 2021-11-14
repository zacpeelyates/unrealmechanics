// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "ItemHolderComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJ_API UItemHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void GetTargetPickup();
	UPROPERTY(EditAnywhere)
	FVector BaseThrowVector;
	UPROPERTY(VisibleAnywhere);
	FVector MaxThrowVector;
	UPROPERTY(VisibleAnywhere)
	FVector MinThrowVector;
	UPROPERTY(VisibleAnywhere)
	FVector ThrowVector;
	FVector PrevThrowVector;
	float ThrowScale;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	UBoxComponent* PickupBox;
	float PickupRange;
	UPROPERTY()
	APickupActor* TargetPickup;
	bool bIsHolding;

	void RequestPickup();
	void RequestThrow();
	void RequestRelease();
	void ResetThrowVector();
	void AddThrowVector(const FVector ThrowDelta);

	
		
};
