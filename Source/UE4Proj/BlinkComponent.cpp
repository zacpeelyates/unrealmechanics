// Fill out your copyright notice in the Description page of Project Settings.


#include "BlinkComponent.h"
#include "CameraPawn.h"
#include "Components/LineBatchComponent.h"

// Sets default values for this component's properties
UBlinkComponent::UBlinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BlinkCheckMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	LineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatchComponent"));
	// ...
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();
	bIsBlinkLocationValid = false;
	bPreview = false;
	bTrace = false;
	BlinkCheckMesh->SetVisibility(false);
	Owner = Cast<ACameraPawn>(GetOwner());

}



// Called every frame
void UBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bPreview)
	{
		Step();
	}
	else
	{
		Steps = BlinkRange;
	}
	if(bTrace)
	{
		Trace();
	}
}



void UBlinkComponent::Trace()
{
	bIsBlinkLocationValid = false;
	LineBatchComponent->Flush();
	BlinkStart = Owner->GetActorLocation();
	BlinkDir = FVector(Owner->GetActiveCamera()->GetForwardVector());
	FHitResult Hit;
	FVector StartLocation = BlinkStart;
	FVector EndLocation = StartLocation + BlinkDir * BlinkRange;
	FColor Color = FColor::Yellow;
	//horizontal trace
	if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility))
	{
		if(Hit.bBlockingHit)
		{
			Color = FColor::Orange;
			EndLocation = Hit.ImpactPoint;
		}
	}
	bPreview = true;
	LineBatchComponent->DrawLine(StartLocation, EndLocation, Color, 1, 5.0f, 2.0f);
}

void UBlinkComponent::Step()
{
	BlinkCheckMesh->SetVisibility(true);
	if(Steps <= 0)
	{
		//check ground
		FColor Color;
		FHitResult Hit;
		FVector StartLocation = BlinkCheckMesh->GetComponentLocation();
		FVector EndLocation = StartLocation + BlinkRange * FVector::DownVector;
		if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility))
		{
			if (Hit.bBlockingHit)
			{
				Color = FColor::Green;
				bIsBlinkLocationValid = true;
				EndLocation = Hit.ImpactPoint;
				BlinkLocation = EndLocation;
				BlinkLocation.Z += 25.0f;
				BlinkCheckMesh->SetWorldLocation(BlinkLocation);
				
			}
			else
			{
				Color = FColor::Red;
				bIsBlinkLocationValid = false;
			}
		}
		LineBatchComponent->DrawLine(StartLocation, EndLocation, Color, 1, 5.0f, 2.0f);
		bPreview = false;
		Steps = BlinkRange;
	}
	Steps-= StepsPerFrame;
	FHitResult OutResult; 
	BlinkCheckMesh->AddWorldOffset(BlinkDir*StepsPerFrame,true,&OutResult);
	if(OutResult.bBlockingHit)
	{
		Steps = 0;
	}

}

void UBlinkComponent::RequestTeleport()
{
	Owner->SetActorLocation(BlinkLocation);
}

void UBlinkComponent::SetTrace(bool bNewTrace)
{
	bTrace = bNewTrace;
	BlinkStart = Owner->GetActorLocation();
	BlinkCheckMesh->SetWorldLocation(BlinkStart);
}

