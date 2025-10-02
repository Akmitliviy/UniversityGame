// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Controllers/ActivePlayerController.h"
#include "UniversityGame/Weapons/Bullet.h"
#include "MainCharacter.generated.h"

class ABaseWeapon;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class UNIVERSITYGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AActivePlayerController* PlayerController;

	// CAMERA BEGIN
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Camera)
	UCameraComponent* CameraComponent;
	// CAMERA END

	// MOVEMENT BEGIN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double Speed = 45;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double MaxScopedSpeed = 27;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	double MaxCrouchSpeed = 20;

	UPROPERTY(BlueprintReadWrite)
	bool ScopeButtonDown = false;

	UPROPERTY(BlueprintReadWrite)
	bool CanMove = true;
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void CustomCrouch(const FInputActionValue& Value);
	
	void Scope(const FInputActionValue& Value);
	// MOVEMENT END

	// INPUT BEGIN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MovementAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* ScopeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Input)
	UInputAction* ReloadAction;
	// INPUT END

	// Shooting BEGIN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<ABaseWeapon>  WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	bool CanShoot = true;

	UPROPERTY()
	ABaseWeapon* Weapon;
	
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void Reload(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReload();
	// Shooting END
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
