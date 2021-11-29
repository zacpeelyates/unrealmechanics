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
	BlinkPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	LineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatchComponent"));
	// ...
}


// Called when the game starts
void UBlinkComponent::BeginPlay()
{
	Super::BeginPlay();
	BlinkPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); //the raytrace was colliding with the previewmesh and breaking everything took me a while to figure that one out
	bIsBlinkLocationValid = false;
	bTrace = false;
	BlinkPreviewMesh->SetVisibility(false);
	Owner = Cast<ACameraPawn>(GetOwner());

}



// Called every frame
void UBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bTrace)
	{
		Trace();
	}
	
}



void UBlinkComponent::Trace()
{
	LineBatchComponent->Flush();
	BlinkStart = Owner->GetActorLocation();
	BlinkDir = FVector(Owner->GetActiveCamera()->GetForwardVector());
	FHitResult Hit;
	FVector StartLocation = BlinkStart;
	FVector EndLocation = StartLocation + BlinkDir * BlinkRange;
	FColor Color = FColor::Yellow;
	bool bHasHit = false;
	//horizontal trace
	if(GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility))
	{
		if(Hit.bBlockingHit)
		{
			Color = FColor::Orange;
			EndLocation = Hit.ImpactPoint;
			FVector SurfaceNormal = (Hit.ImpactNormal + Hit.ImpactPoint);
			SurfaceNormal.Normalize();
			bHasHit = true;
			EndLocation -= BlinkDir * WallOffset;
			
		}
	}
	LineBatchComponent->DrawLine(StartLocation, EndLocation, Color, 1, 5.0f);
	if(bHasHit)
	{
		//if horizontal trace hits a wall, we only need to do one vertical trace to check if there is valid floor by the wall
		FVector VertEndLocation = EndLocation + FVector::DownVector * BlinkRange;
		if(GetWorld()->LineTraceSingleByChannel(Hit,EndLocation,VertEndLocation,ECC_Visibility))
		{
			if(Hit.bBlockingHit)
			{
				Color = FColor::Green;
				VertEndLocation = Hit.ImpactPoint;
				BlinkLocation = VertEndLocation + ZOffset;
				bIsBlinkLocationValid = true;
			}
		}
		LineBatchComponent->DrawLine(EndLocation, VertEndLocation, Color, 1, 5.0f);
	}
	else 
	{
		//vertical traces
		for (int i = VerticalTraceSpacing; i <= BlinkRange; i += VerticalTraceSpacing)
		{
			FVector VertStartLocation = StartLocation + BlinkDir * i;
			FVector VertEndLocation = VertStartLocation + FVector::DownVector * i/VerticalTraceDivisor;

			if (GetWorld()->LineTraceSingleByChannel(Hit, VertStartLocation, VertEndLocation, ECC_Visibility))
			{
				if (Hit.bBlockingHit)
				{
					Color = FColor::Green;
					VertEndLocation = Hit.ImpactPoint;
					BlinkLocation = VertEndLocation + ZOffset;
					bIsBlinkLocationValid = true;
				}
				else
				{
					Color = FColor::Red;
				}
			}
			LineBatchComponent->DrawLine(VertStartLocation, VertEndLocation, Color, 1, 5.0f);
		}
	}
	if(bIsBlinkLocationValid)
	{
		BlinkPreviewMesh->SetWorldLocation(BlinkLocation);
		BlinkPreviewMesh->SetVisibility(true);
	}

}

void UBlinkComponent::RequestTeleport()
{
	if (bIsBlinkLocationValid) {
		Owner->SetActorLocation(BlinkLocation);
	}
}

void UBlinkComponent::SetTrace(bool bNewTrace)
{

	bTrace = bNewTrace;
	BlinkPreviewMesh->SetVisibility(false);
	bIsBlinkLocationValid = false;
	BlinkStart = Owner->GetActorLocation();
	LineBatchComponent->Flush();
}

