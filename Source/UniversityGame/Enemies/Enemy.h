// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

struct FInputActionValue;
class ABaseWeapon;

UCLASS()
class UNIVERSITYGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// MOVEMENT BEGIN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double Speed = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double MaxScopedSpeed = 27;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double MaxCrouchSpeed = 20;

	UPROPERTY(BlueprintReadWrite)
	bool CanMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	FVector TargetPointStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	FVector TargetPointEnd;
	// MOVEMENT END

	// Shooting BEGIN
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<ABaseWeapon>  WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	bool CanShoot = true;

	UPROPERTY()
	ABaseWeapon* Weapon;
	
	void Fire();

	void Reload();
	
	UPROPERTY(BlueprintReadWrite)
	bool IsScoped = false;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReload();
	// Shooting END
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnDeath();
	
	UFUNCTION(BlueprintCallable)
	void OnDeathFinal();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBeingShot();

	// AI START
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AI)
	UBehaviorTree* BehaviorTree;
	// AI END
	
public:
	
	UPROPERTY(EditAnywhere, Category=Health)
	float Health;

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, Category=Health)
	bool IsDying = false;

	UPROPERTY(BlueprintReadWrite, Category=Reactions)
	bool CanPlayShotAnim = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Editor)
	bool IsImmortal = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UBehaviorTree* GetBehaviorTree() const;

	FVector GetTargetPointStart() const;
	
	FVector GetTargetPointEnd() const;
};
