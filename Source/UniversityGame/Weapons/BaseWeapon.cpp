// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "Bullet.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSubclassOf<ABullet> ABaseWeapon::GetBulletClass() const
{
	return BulletClass;
}

FVector ABaseWeapon::GetMuzzleLocation() const
{
	if (SkeletalMeshComponent)
	{
		return SkeletalMeshComponent->GetSocketLocation(TEXT("b_gun_muzzleflashSocket"));
	}
	return GetActorLocation();
}

float ABaseWeapon::GetFireRate() const
{
	return FireRate;
}

void ABaseWeapon::Fire(const FRotator& BulletInitialRotation, const FVector& BulletLaunchDirection)
{
	if (BulletClass == nullptr) return;

	if (!CanFire()) return;
	
	const FVector MuzzleLocation = GetMuzzleLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	MagazineAmmoCount--;
	if (ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, BulletInitialRotation, SpawnParams); Bullet != nullptr)
	{
		Bullet->OnFired(BulletLaunchDirection);
	}
	
	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&ABaseWeapon::ResetFireCooldown,
		FireRate,
		false);
}

bool ABaseWeapon::CanFire() const
{
	return bCanFire && MagazineAmmoCount > 0;
}

void ABaseWeapon::ResetFireCooldown()
{
	bCanFire = true;
}

FVector ABaseWeapon::GetWeaponDirection() const
{
	return SkeletalMeshComponent->GetForwardVector();
}

bool ABaseWeapon::CanReload() const
{
	return UnequippedAmmoCount > 0 && MagazineAmmoCount < MagazineAmmoCapacity;
}

void ABaseWeapon::Reload()
{
	if (const int ToFill = MagazineAmmoCapacity - MagazineAmmoCount; UnequippedAmmoCount >= ToFill)
	{
		MagazineAmmoCount += ToFill;
		UnequippedAmmoCount -= ToFill;
	}
	else
	{
		MagazineAmmoCount += UnequippedAmmoCount;
		UnequippedAmmoCount = 0;
	}
}
