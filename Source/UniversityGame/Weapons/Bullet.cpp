// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/DecalActor.h"
#include "Kismet/GameplayStatics.h"
#include "UniversityGame/Character/MainCharacter.h"

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

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
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
		UE_LOG(LogTemp, Warning, TEXT("Hit WorldStatic: %s"), *(Other->GetName()));
	}
	
	if (Other->CanBeDamaged())
	{
		const FDamageEvent DamageEvent;
		UE_LOG(LogTemp, Warning, TEXT("Applying Damage to : %s"), *(Other->GetName()));
		Other->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
	}
	
	Destroy();
}

