// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABullet;

UCLASS()
class UNIVERSITYGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* SkeletalMeshComponent ;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	int GeneralAmmoCapacity = 120;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	int MagazineAmmoCapacity = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	int UnequippedAmmoCount = 120;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	int MagazineAmmoCount = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
	float FireRate = 0.16f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
	TSubclassOf<ABullet> BulletClass;
	
	bool bCanFire = true;

	FTimerHandle FireRateTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<ABullet> GetBulletClass() const;

	FVector GetMuzzleLocation() const;

	float GetFireRate() const;

	void Fire(const FRotator& BulletInitialRotation, const FVector& BulletLaunchDirection);

	bool CanFire() const;

	void ResetFireCooldown();

	FVector GetWeaponDirection() const;

	bool CanReload() const;

	void Reload();
};
