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
	PickupOffset = FVector(100.0f,0.0f,100.0f);
	PreviewCopy = nullptr;
	Holder = nullptr;
	BasePreviewTime = 3.0f;
	PreviewTimer = BasePreviewTime;
	ThrowZOffset = 15.0f;
	ThrowForce = 25000.0f;
	bCanCopy = true;
	bCanPickup = true;
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetSimulatePhysics(true); \
	defaultCollision = MeshComponent->GetCollisionEnabled();
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
	MeshComponent->SetCollisionEnabled(defaultCollision);
	MeshComponent->SetSimulatePhysics(true);
	PreviewTimer = BasePreviewTime;
	if(PreviewCopy != nullptr)
	{
		PreviewCopy->Destroy();
		PreviewCopy = nullptr;
	}
}

void APickupActor::Throw()
{
	if (Holder != nullptr) {
		FVector ThrowVector = Holder->GetActorForwardVector() * ThrowForce;
		ThrowVector.Z += ThrowZOffset;
		Release();
		MeshComponent->AddImpulse(ThrowVector);
	}
}

void APickupActor::Preview()
{
	if (PreviewCopy == nullptr && bCanCopy)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Template = this;
		const FVector L = GetActorLocation();
		const FRotator R = GetActorRotation();
		PreviewCopy = (APickupActor*)GetWorld()->SpawnActor(APickupActor::StaticClass(), &L, &R, SpawnParameters);
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PreviewCopy->MeshComponent->SetCollisionEnabled(defaultCollision);
		PreviewCopy->MeshComponent->SetSimulatePhysics(false);
		PreviewCopy->bCanPickup = false;
		PreviewCopy->bCanCopy = false; //prevent copies from making copies which would cause the world to end or something
		PreviewCopy->Holder = Holder;
		PreviewCopy->AttachToActor(Holder, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		PreviewCopy->MeshComponent->SetRelativeLocationAndRotation(Holder->GetActorLocation(),Holder->GetActorRotation());

		
		UMaterialInstanceDynamic* mid =  PreviewCopy->MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
		mid->BlendMode = BLEND_Translucent;
		mid->SetScalarParameterValue("OPACITY", 0.5);
		
		PreviewTimer = BasePreviewTime;
		PreviewCopy->Throw();
	}
	
	if(PreviewTimer <= 0)
	{
		MeshComponent->SetCollisionEnabled(defaultCollision);
		PreviewTimer = BasePreviewTime;
		PreviewCopy->Destroy();
		PreviewCopy = nullptr;
	}


}







