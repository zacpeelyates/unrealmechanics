// Fill out your copyright notice in the Description page of Project Settings.


#include "ProxDoor.h"
#include "Kismet/KismetMathLibrary.h"

AProxDoor::AProxDoor() : Super()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AProxDoor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AProxDoor::OnOverlapEnd);
	TriggerBox->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(MeshOffset);
}


void AProxDoor::BeginPlay()
{
	Super::BeginPlay();
	DirMultiplier = 1.0f;
}

void AProxDoor::OnOverlapBegin(UPrimitiveComponent* OverlapC, AActor* OtherA, UPrimitiveComponent* OtherC,int32 OtherI, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherA == this || OtherA == nullptr || OtherC == nullptr) return;
	FVector Dir = GetActorLocation() - OtherA->GetActorLocation();
	Dir = UKismetMathLibrary::LessLess_VectorRotator(Dir, GetActorRotation());
	const FRotator r = OpenTransform.GetRotation().Rotator();
	if (Dir.X < 0.0f)
	{
		DirMultiplier = -1.0f;
	}
	else
	{
		DirMultiplier = 1.0f;
	}
	Open();
}

void AProxDoor::OnOverlapEnd(UPrimitiveComponent* OverlapC, AActor* OtherA, UPrimitiveComponent* OtherC, int32 OtherI)
{
	if (OtherA == this || OtherA == nullptr || OtherC == nullptr) return;
	Close();
}

void AProxDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProxDoor::Open()
{
	GoalTransform = OpenTransform;
	GoalTransform.Rotator() *= DirMultiplier;
	bIsOpen = true;
	bIsTransitioning = true;
}

void AProxDoor::Close()
{
	GoalTransform = CloseTransform;
	bIsOpen = false;
	bIsTransitioning = true;
}
