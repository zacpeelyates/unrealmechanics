// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHolderComponent.h"

// Sets default values for this component's properties
UItemHolderComponent::UItemHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PickupRange = 200.0f;
	TargetPickup = nullptr;
	bIsHolding = false;
	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBoxComponent"));
	PickupBox->InitBoxExtent(FVector(PickupRange));;

}


// Called when the game starts
void UItemHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemHolderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bIsHolding)
	{
		GetTargetPickup();
	}


}

void UItemHolderComponent::RequestPickup()
{
	if(TargetPickup != nullptr && !bIsHolding)
	{
		TargetPickup->Pickup(GetOwner());
		bIsHolding = true;
	}
}

void UItemHolderComponent::RequestThrow()
{
	if (TargetPickup != nullptr && bIsHolding)
	{
		TargetPickup->Throw();
		TargetPickup = nullptr;
		bIsHolding = false;
	}
}


void UItemHolderComponent::RequestRelease()
{
	if (TargetPickup != nullptr && bIsHolding)
	{
		TargetPickup->Release();
		TargetPickup = nullptr;
		bIsHolding = false;
	}
}


void UItemHolderComponent::GetTargetPickup()
{
	TargetPickup = nullptr;
	TArray<AActor*> Actors;
	PickupBox->GetOverlappingActors(Actors);
	if (Actors.Num() == 0) return;
	float ClosestDistance = FLT_MAX;
	for (AActor* Actor : Actors)
	{
		if (Actor->IsA(APickupActor::StaticClass()))
		{
			const float CurrentDistance = FMath::Abs(FVector::Dist(GetOwner()->GetActorTransform().GetLocation(), Actor->GetActorTransform().GetLocation()));
			if (CurrentDistance < ClosestDistance)
			{
				ClosestDistance = CurrentDistance;
				TargetPickup = static_cast<APickupActor*>(Actor);
			}

		}
	}
}


