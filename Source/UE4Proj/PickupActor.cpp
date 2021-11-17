// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupActor.h"


// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Set Root Component
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	//Set Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	//Set Parameters 
	PreviewCopy = nullptr;
	Holder = nullptr;
	BasePreviewTime = 3.0f;
	PreviewTimer = BasePreviewTime;
	bCanCopy = true;
	bCanPickup = true;
	LineBatchComponent = CreateDefaultSubobject <ULineBatchComponent>(TEXT("LineBatchComponent"));
	ColorPoint = FVector(0.0f, 0.0f, 0.0f);

}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetSimulatePhysics(true); \
	DefaultCollisionType = MeshComponent->GetCollisionEnabled();
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Holder != nullptr)
	{
		MeshComponent->SetRelativeLocationAndRotation(Holder->GetActorLocation(),Holder->GetActorRotation());
		MeshComponent->AddLocalOffset(PickupOffset);
		if(bCanCopy) Preview();
	}
	prevLocation = MeshComponent->GetComponentLocation();
	PreviewTimer -= DeltaTime;
}
void APickupActor::Pickup(AActor* HoldActor)
{
	if (HoldActor != nullptr && bCanPickup) 
	{
		MeshComponent->SetSimulatePhysics(false);
		Holder = HoldActor;
		AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void APickupActor::Release()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Holder = nullptr;
	MeshComponent->SetCollisionEnabled(DefaultCollisionType);
	MeshComponent->SetSimulatePhysics(true);
	PreviewTimer = BasePreviewTime;
	if(PreviewCopy != nullptr)
	{
		PreviewCopy->Destroy();
		PreviewCopy = nullptr;
	}
	LineBatchComponent->Flush();
}

void APickupActor::Throw()
{
	if (Holder != nullptr) {
		const FVector ThrowF = Holder->GetActorForwardVector() * ThrowVector.X;
		const FVector ThrowR = Holder->GetActorRightVector() * ThrowVector.Y;
		const FVector ThrowU = Holder->GetActorUpVector() * ThrowVector.Z;
		Release();
		MeshComponent->AddImpulse(ThrowF + ThrowU + ThrowR);
	}
}


void APickupActor::Preview()
{
	if (PreviewCopy == nullptr && bCanCopy)
	{
		//create and show preview copy
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Template = this;
		const FVector L = GetActorLocation();
		const FRotator R = GetActorRotation();
		PreviewCopy = (APickupActor*)GetWorld()->SpawnActor(APickupActor::StaticClass(), &L, &R, SpawnParameters);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PreviewCopy->MeshComponent->SetCollisionEnabled(DefaultCollisionType);
		PreviewCopy->MeshComponent->SetSimulatePhysics(false);
		PreviewCopy->bCanPickup = false;
		PreviewCopy->bCanCopy = false; //prevent copies from making copies which would cause the world to end or something
		PreviewCopy->Holder = Holder;
		PreviewCopy->AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PreviewCopy->MeshComponent->SetRelativeLocationAndRotation(Holder->GetActorLocation(),Holder->GetActorRotation());
		PreviewCopy->MeshComponent->SetMaterial(0,PreviewMaterial);
		PreviewTimer = BasePreviewTime;
		PreviewCopy->Throw();
	}
	else if(PreviewTimer <= 0)
	{
		//reset preview copy
		MeshComponent->SetCollisionEnabled(DefaultCollisionType);
		PreviewTimer = BasePreviewTime;
		PreviewCopy->Destroy();
		PreviewCopy = nullptr;
	}
	else if(PreviewCopy != nullptr)
	{
		LineBatchComponent->DrawPoint(PreviewCopy->MeshComponent->GetComponentLocation(),FLinearColor(ColorPoint.X,ColorPoint.Y,ColorPoint.Z),5, 1, PreviewTimer);
	}
}



void APickupActor::SetThrowVector(FVector NewThrowVector)
{
	if (ThrowVector != NewThrowVector) {
		ThrowVector = NewThrowVector;
		PreviewTimer = 0; //force preview to reset when new throw values are given 
	}
}

void APickupActor::SetColorPoint(FVector NewColor)
{
	ColorPoint = NewColor;
}








