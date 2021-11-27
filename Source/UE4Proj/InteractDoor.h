// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorActor.h"
#include "InteractDoor.generated.h"

/**
 * 
 */
UCLASS()
class UE4PROJ_API AInteractDoor : public ADoorActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:

	virtual void InteractionBegin();
	virtual void InteractionEnd() {}; //no behaviour by default, but still may be called so cant mark as pure virtual without crash
	virtual void Tick(float DeltaTime) override;

};
