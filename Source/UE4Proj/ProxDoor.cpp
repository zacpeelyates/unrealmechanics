// Fill out your copyright notice in the Description page of Project Settings.


#include "ProxDoor.h"
#include "Kismet/KismetMathLibrary.h"

AProxDoor::AProxDoor() : Super()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AProxDoor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AProxDoor::OnOverlapEnd);
	TriggerBox->SetupAttachment(RootComponent);
}


void AProxDoor::BeginPlay()
{
	Super::BeginPlay();

}

void AProxDoor::OnOverlapBegin(UPrimitiveComponent* OverlapC, AActor* OtherA, UPrimitiveComponent* OtherC,int32 OtherI, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherA == this || OtherA == nullptr || OtherC == nullptr) return;
	bEnteredThisFrame = true;
	SetDir(OtherA->GetActorLocation());
	Open();
}

void AProxDoor::OnOverlapEnd(UPrimitiveComponent* OverlapC, AActor* OtherA, UPrimitiveComponent* OtherC, int32 OtherI)
{
	if (OtherA == this || OtherA == nullptr || OtherC == nullptr || bEnteredThisFrame) return;
	Close();
}


void AProxDoor::Open()
{
	Super::Open();
}

void AProxDoor::Close()
{
	Super::Close();
}


void AProxDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bEnteredThisFrame = false;
}
