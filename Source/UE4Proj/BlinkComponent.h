// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlinkComponent.generated.h"


class ACameraPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJ_API UBlinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlinkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	bool bIsBlinkLocationValid;
	bool bPreview;
	bool bTrace;
	FVector BlinkLocation;
	FVector BlinkStart;
	FVector BlinkDir;
	UPROPERTY(EditAnywhere)
	int StepsPerFrame;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
		int BlinkRange;
	UPROPERTY(VisibleAnywhere)
		int Steps;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlinkCheckMesh;
	UPROPERTY(EditAnywhere)
		ULineBatchComponent* LineBatchComponent;
	UPROPERTY(VisibleAnywhere)
		ACameraPawn* Owner;

	void Trace();
	void Step();
	void RequestTeleport();
	void SetTrace(bool bNewTrace);

		
};
