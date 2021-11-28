// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlinkComponent.generated.h"


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
	bool bIsBlinkPositionValid;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
		float BlinkRange;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlinkPreviewMesh;
	UPROPERTY(EditAnywhere)
		ULineBatchComponent* LineBatchComponent;

	void Trace();

		
};
