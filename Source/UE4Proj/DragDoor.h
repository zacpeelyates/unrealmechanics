// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractDoor.h"
#include "DragDoor.generated.h"

/**
 * 
 */
UCLASS()
class UE4PROJ_API ADragDoor : public AInteractDoor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	AActor* Interactor;
public:

	virtual void InteractionBegin() override;
	virtual void InteractionEnd() override; //no behaviour by default, but still may be called so cant mark as pure virtual without crash
	void SetInteractor(AActor* Other);
	virtual void Tick(float DeltaTime) override;
	bool bIsInteracting;

};
