// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DecalActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (SphereComponent == nullptr)
	{
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		SphereComponent->InitSphereRadius(2.f);
		SphereComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		RootComponent = SphereComponent;
	}

	// Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// Mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(SphereComponent);

	SphereComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnFired(const FVector& Direction) const
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed * GetWorld()->GetDeltaSeconds();
}

void ABullet::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	const UDecalComponent* Decal = UGameplayStatics::SpawnDecalAttached(
		DecalMaterial,
		FVector(2.f, 2.f, 2.f),
		OtherComp,
		NAME_None,
		HitLocation,
		HitNormal.Rotation(),
		EAttachLocation::KeepWorldPosition,
		10.f
	);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, TEXT("Target hit: " + Other->GetName()));

		if (Decal != nullptr)
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Yellow, TEXT("Decal created: " + Decal->GetName()));
	}
	Destroy();
}

