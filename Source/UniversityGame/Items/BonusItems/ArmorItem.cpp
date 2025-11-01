// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UniversityGame/Character/MainCharacter.h"

// Sets default values
AArmorItem::AArmorItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (SphereComponent == nullptr)
	{
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		SphereComponent->InitSphereRadius(30.f);
		SphereComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		RootComponent = SphereComponent;
	}

	// Mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);

	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("BonusItem"));

}

// Called when the game starts or when spawned
void AArmorItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SphereComponent->AddRelativeRotation(FRotator(0.0f, ItemRotationSpeed, 0.0f));
	SphereComponent->AddRelativeLocation(FVector(0.0f, 0.0f, UKismetMathLibrary::Sin(CurrentAscendingRadianValue + DeltaTime) * ItemMaxAscendingHeight));
	CurrentAscendingRadianValue += ItemAscendingSpeed;
}

void AArmorItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	
	if (AMainCharacter* Player = Cast<AMainCharacter>(OtherActor); Player != nullptr)
	{
		Player->MakeInvincibleFor(InvincibilityDuration);
		Destroy();
	}
}

