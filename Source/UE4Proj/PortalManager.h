// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerController.h"
#include "GameFramework/Actor.h"
#include "PortalManager.generated.h"


//forward declares
class APortalActor;
class ACameraPawn;

UCLASS()
class UE4PROJ_API APortalManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	UMaterial* BasePortalMaterial;

private:
	UPROPERTY(VisibleAnywhere)
	ACustomPlayerController* PlayerController;
	int32 ScreenX, ScreenY;

	void HandleTeleport(APortalActor* PortalActor, AActor* TeleportActor);
	APortalActor* GetClosestPortal();
	void Init();

	
};

