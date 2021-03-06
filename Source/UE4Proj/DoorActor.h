// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class UE4PROJ_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMeshComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HingeMeshComponent;

	UPROPERTY(EditAnywhere)
		USceneComponent* DoorParent;

private:
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Open();
	virtual void Close();
	void SetDir(FVector OtherLocation);
		
	

	bool bIsOpen;
	bool bIsTransitioning;

	UPROPERTY(EditAnywhere)
		FTransform OpenTransform;

	UPROPERTY(VisibleAnywhere)
	FTransform CloseTransform;

	UPROPERTY(VisibleAnywhere)
	FTransform GoalTransform;

	int Dir;
};
