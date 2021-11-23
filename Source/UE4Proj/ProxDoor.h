// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "DoorActor.h"
#include "ProxDoor.generated.h"

/**
 * 
 */
UCLASS()
class UE4PROJ_API AProxDoor : public ADoorActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AProxDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlapC, class AActor* OtherA, class UPrimitiveComponent* OtherC, int32 OtherI, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlapC, class AActor* OtherA, class UPrimitiveComponent* OtherC, int32 OtherI);
	bool bEnteredThisFrame;
	UPROPERTY(VisibleAnywhere)
	int Dir;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Open() override;
	virtual void Close() override;
	
};
