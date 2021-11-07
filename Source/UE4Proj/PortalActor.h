// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	//setters
	void SetEnabled(bool bIn);
	UFUNCTION(BlueprintCallable)
	void SetLinkedPortal(APortalActor* NewLinkedPortal);
	//portal functions
	UFUNCTION(BlueprintCallable)
	bool IsInPortal(FVector TargetLocation, FVector PortalLocation, FVector PortalNormal);
	void TeleportActor(AActor* TeleportActor);
	//portal render texture functions (implemented in blueprint)
	UFUNCTION(BlueprintImplementableEvent)
	void SetRenderTexture(UTexture* RenderTexture);
	UFUNCTION(BlueprintImplementableEvent)
	void ClearRenderTexture();

private:
	bool bIsEnabled;
	UPROPERTY(EditAnywhere);
	APortalActor* LinkedPortal;
	FVector LastTargetPosition;
	bool IsLastPositionInFrontOfPortal;
	UFUNCTION(BlueprintCallable)
	bool IsInBounds(FVector Location, UBoxComponent* Bounds);
	UFUNCTION(BlueprintCallable)
	APortalManager* GetPortalManager();
};