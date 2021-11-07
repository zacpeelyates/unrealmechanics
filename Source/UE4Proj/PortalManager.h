// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomPlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
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
	UFUNCTION(BlueprintCallable)
	void HandleTeleport(APortalActor* PortalActor, AActor* TeleportActor);
	void Initialize();
	void Update(float DeltaTime);
	APortalActor* UpdateClosestPortal();
	void UpdateView(APortalActor* PortalActor);

private:
	void CreateRenderTarget();
	UPROPERTY(VisibleAnywhere)
	ACustomPlayerController* PlayerController;
	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;
	UPROPERTY(Transient)
	UTextureRenderTarget2D* PortalTexture;
	int32 ScreenX, ScreenY;
};

