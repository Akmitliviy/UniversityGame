// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (SphereComponent == nullptr)
	{
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		SphereComponent->InitSphereRadius(2.f);
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

void ABullet::OnFired(const FVector& Direction, const FVector& TargetLocation)
{
    InitialDirection = Direction;
    TargetPoint = TargetLocation;
    StartLocation = GetActorLocation();
    
    TotalDistance = FVector::Dist(StartLocation, TargetPoint);
    
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = InitialDirection * ProjectileMovement->InitialSpeed;
        ProjectileMovement->bIsHomingProjectile = false;
    }
}

void ABullet::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

	UpdateTrajectory();
}

void ABullet::UpdateTrajectory()
{
	const FVector CurrentLocation = GetActorLocation();
    const float CurrentDistance = FVector::Dist(StartLocation, StartLocation + UKismetMathLibrary::ProjectVectorOnToVector(CurrentLocation - StartLocation, TargetPoint - StartLocation));
	
    const float ProgressToTarget = FMath::Clamp(CurrentDistance / TotalDistance, 0.f, 1.f);

	const FVector Offset = CalculateSpiralOffset(ProgressToTarget);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			INDEX_NONE,
			1.f,
			FColor::Yellow,
			TEXT("Offset vector is: " + Offset.ToString()));
	}
	const FVector NewDirection = (GetActorForwardVector() + Offset).GetSafeNormal(); 
	const FVector NewLocation = CurrentLocation + NewDirection;
	SetActorLocation(NewLocation);
	
    if (ProjectileMovement)
    {
	    if (!NewDirection.IsNearlyZero())
        {
            ProjectileMovement->Velocity = NewDirection * ProjectileMovement->InitialSpeed;
        }
    }
}

FVector ABullet::CalculateSpiralOffset(const float Progress) const
{
	const float TakePercentage = 1.f / Waves;
	const float LocalProgress = FMath::Clamp(FMath::Frac(Progress / TakePercentage), 0.f, 1.f);
	const float AngleDegree = FMath::Lerp(0, 360, LocalProgress);
	
    const float Angle = AngleDegree * PI / 180.f;

    const FVector Right = GetActorRightVector();
    const FVector Up = FVector::CrossProduct(GetActorForwardVector(), Right).GetSafeNormal();

    const float x = FMath::Cos(Angle) * 50.f;
    const float y = FMath::Sin(Angle) * 50.f;
    
    return Right * x + Up * y;
}

void ABullet::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	UGameplayStatics::SpawnDecalAttached(
		DecalMaterial,
		FVector(2.f, 2.f, 2.f),
		OtherComp,
		NAME_None,
		HitLocation,
		HitNormal.Rotation(),
		EAttachLocation::KeepWorldPosition,
		10.f
	);
	
	Destroy();
}

