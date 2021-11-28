// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"

#include "Components/LineBatchComponent.h"

// Sets default values for this component's properties
UBlinkComponent::UBlinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




// Called every frame
void UBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBlinkComponent::Trace()
{
	FHitResult Hit;
	AActor* Owner = GetOwner();
	FVector StartLocation = Owner->GetActorLocation();
	FVector EndLocation = StartLocation + (Owner->GetActorForwardVector() * BlinkRange);
	if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility))
	{
		if(Hit.bBlockingHit)
		{
			LineBatchComponent->DrawLine(StartLocation, EndLocation, FColor::Black, 1);
		}
	}

}

