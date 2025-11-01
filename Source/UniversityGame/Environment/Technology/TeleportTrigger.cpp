// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTrigger.h"

#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "UniversityGame/Character/MainCharacter.h"

// Sets default values
ATeleportTrigger::ATeleportTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (BoxComponent == nullptr)
	{
		BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		BoxComponent->InitBoxExtent(FVector(34.f, 40.f, 10.f));
		BoxComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		RootComponent = BoxComponent;
	}

	// Mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(BoxComponent);

	BoxComponent->BodyInstance.SetCollisionProfileName(TEXT("BonusItem"));

}	

// Called when the game starts or when spawned
void ATeleportTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (AMainCharacter* Player = Cast<AMainCharacter>(OtherActor); Player != nullptr && Destination != nullptr)
	{
		Player->TeleportToLocation(Destination->GetActorLocation());
	}
}

