// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PortalManager.h"
#include "Components/BoxComponent.h"
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
	//getters
	bool IsEnabled();
	APortalActor* GetLinkedPortal();
	UTextureRenderTarget2D* GetRenderTexture();
	UStaticMeshComponent* GetPortalPlaneMesh();
	USceneCaptureComponent2D* GetSceneCaptureComponent();
	//setters
	void SetEnabled(bool bIn);
	void SetLinkedPortal(APortalActor* NewLinkedPortal);
	void SetRenderTexture(UTextureRenderTarget2D* NewRenderTexture);
	void SetMaterialInstance(UMaterialInstanceDynamic* NewMaterialInstanceDynamic);

	//portal functions
	bool IsInPortal(FVector TargetLocation, FVector PortalLocation, FVector PortalNormal);
	void TeleportActor(AActor* TeleportActor);
private:
	bool bIsEnabled;
	UPROPERTY(EditAnywhere)
	APortalActor* LinkedPortal;
	FVector LastTargetPosition;
	bool IsLastPositionInFrontOfPortal;
	bool IsInBounds(FVector Location, UBoxComponent* Bounds);

	UPROPERTY(VisibleAnywhere)
	UTextureRenderTarget2D* RenderTexture;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PortalPlaneMesh;
	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;
	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* MaterialInstance;


};