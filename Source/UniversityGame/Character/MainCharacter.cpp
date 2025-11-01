// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ProfilingDebugging/CookStats.h"
#include "UniversityGame/Weapons/BaseWeapon.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(true);
	
	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(CameraComponent != nullptr);
	CameraComponent->SetupAttachment(GetMesh(), TEXT("head"));
	CameraComponent->bUsePawnControlRotation = true;

	// Movement
	if (ACharacter::GetMovementComponent())
	{
		ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}

	Health = MaxHealth;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController = Cast<AActivePlayerController>(GetController()), PlayerController == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Could not cast to AActivePlayerController"));
		}
	}

	ScopeButtonDown = false;
	CanMove = true;

	AcquireWeapon(WeaponClass);
	ChangeHealth(MaxHealth);
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (!CanMove) return;

	const UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (MovementComponent == nullptr) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector MovementMath = FVector(MovementVector.Y, MovementVector.X, 0.f);
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector Vector = UKismetMathLibrary::Normal(FRotationMatrix(YawRotation).TransformVector(MovementMath));
	const double SpeedModifier = MovementComponent->IsCrouching()
		? UKismetMathLibrary::Clamp(Speed, 0, MaxCrouchSpeed)
		: ScopeButtonDown
			? UKismetMathLibrary::Clamp(Speed, 0, MaxScopedSpeed)
			: Speed;
	
	AddMovementInput(Vector * SpeedModifier * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMainCharacter::CustomCrouch(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Crouch();
	}else
	{
		UnCrouch();
	}
}

void AMainCharacter::Scope(const FInputActionValue& Value)
{
	if (const bool NewValue = Value.Get<bool>(); ScopeButtonDown != NewValue)
	{
		ScopeButtonDown = NewValue;
		if (ScopeButtonDown)
		{
			PlayerController->HideCrosshair();
		}else
		{
			PlayerController->ShowCrosshair();
		}
	}
}

void AMainCharacter::Fire(const FInputActionValue& Value)
{
	if (WeaponClass == nullptr) return;
	if (!CanShoot) return;

	//const FRotator CameraRotation = GetBaseAimRotation();
	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector CameraDirection = CameraComponent->GetForwardVector();

	const FVector MuzzleLocation = Weapon->GetMuzzleLocation();
	const FVector SightLocation = Weapon->GetSightLocation();
	float TraceDistance = 10000.f;

	FVector TraceDirection;

	if (ScopeButtonDown)
	{
		TraceDirection = SightLocation - CameraLocation;
	}else
	{
		TraceDirection = CameraDirection;
	}
	TraceDirection.Normalize();
	TraceDirection *= TraceDistance;
	
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	
	FHitResult RV_Hit;
	
	GetWorld()->LineTraceSingleByChannel(RV_Hit, CameraLocation, CameraLocation + TraceDirection, ECC_Visibility, RV_TraceParams);

	FVector LaunchDirection;
	if (RV_Hit.bBlockingHit)
	{
		LaunchDirection = UKismetMathLibrary::Normal(RV_Hit.ImpactPoint - MuzzleLocation);
	}else
	{
		LaunchDirection = UKismetMathLibrary::Normal(CameraLocation + TraceDirection - MuzzleLocation);
	}
	Weapon->Fire(LaunchDirection.Rotation(), LaunchDirection);

	PlayerController->UpdateUnequippedAmmoCount(Weapon->GetUnequippedAmmoCount());
	PlayerController->UpdateAmmoInMagCount(Weapon->GetMagazineAmmoCount());
}

void AMainCharacter::Reload(const FInputActionValue& Value)
{
	if (Weapon->CanReload())
	{
		CanShoot = false;
		Weapon->Reload();
		OnReload();
		
		PlayerController->UpdateUnequippedAmmoCount(Weapon->GetUnequippedAmmoCount());
		PlayerController->UpdateAmmoInMagCount(Weapon->GetMagazineAmmoCount());
	}

}

void AMainCharacter::Pick(const FInputActionValue& Value)
{
	
	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector CameraDirection = CameraComponent->GetForwardVector();
	
	FCollisionObjectQueryParams RV_ObjectQueryParams = FCollisionObjectQueryParams(
		ECC_TO_BITFIELD(ECC_GameTraceChannel2)
	);
	
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.bDebugQuery = true;

	FCollisionShape RV_TraceShape;
	RV_TraceShape.SetSphere(50.f);
	
	FHitResult RV_Hit;
	GetWorld()->SweepSingleByObjectType(RV_Hit, CameraLocation, CameraLocation + CameraDirection * 100.f, FQuat::Identity, RV_ObjectQueryParams, RV_TraceShape, RV_TraceParams);

	FHitResult DebugHit;
	FLinearColor DebugTraceColor = FLinearColor::Green;
	FLinearColor DebugHitColor = FLinearColor::Red;
	DrawDebugSphereTraceSingle(GetWorld(), CameraLocation, CameraLocation + CameraDirection * 100.f, 50.f, EDrawDebugTrace::ForOneFrame, true, DebugHit, DebugTraceColor, DebugHitColor, 3.f);

	
	AActor* HitActor = RV_Hit.GetActor();
	if (ABaseWeapon* HitWeapon = Cast<ABaseWeapon>(HitActor); HitWeapon != nullptr)
	{
		if (Weapon != nullptr)
		{
			DropWeapon();
		}
		AcquireWeapon(HitWeapon->GetClass(), HitWeapon);
		HitWeapon->Destroy();
	}
	
}

void AMainCharacter::DropWeapon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const FTransform WeaponTransform = Weapon->GetActorTransform();

	if (ABaseWeapon* DroppedWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor(WeaponClass, &WeaponTransform, SpawnParams));
		DroppedWeapon != nullptr
	)
	{
		DroppedWeapon->SkeletalMeshComponent->SetSimulatePhysics(true);
		DroppedWeapon->CopyFrom(Weapon);
	
		Weapon->Destroy();
		Weapon = nullptr;
	}
}

ABaseWeapon* AMainCharacter::AcquireWeapon(UClass* NewWeaponClass, const ABaseWeapon* NewWeapon)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(Weapon = GetWorld()->SpawnActor<ABaseWeapon>(
			NewWeaponClass,
			FVector(0.f, 0.f, 0.f),
			FRotator(0.f, 0.f, 0.f),
			SpawnParams
		);
		Weapon!= nullptr
	)
	{
		Weapon->SetActorEnableCollision(false);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), Weapon->GetSocketName());

		if (NewWeapon != nullptr)
		{
			Weapon->CopyFrom(NewWeapon);
		}
		Weapon->SetInfiniteAmmo(false);
		WeaponClass = NewWeaponClass;
		
		PlayerController->UpdateMagCapacity(Weapon->GetMagazineAmmoCapacity());
		PlayerController->UpdateAmmoInMagCount(Weapon->GetMagazineAmmoCount());
		PlayerController->UpdateGeneralAmmoCapacity(Weapon->GetGeneralAmmoCapacity());
		PlayerController->UpdateUnequippedAmmoCount(Weapon->GetUnequippedAmmoCount());

		return Weapon;
	}
	return nullptr;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
		Input->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::CustomCrouch);
		Input->BindAction(ScopeAction, ETriggerEvent::Triggered, this, &AMainCharacter::Scope);
		Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMainCharacter::Fire);
		Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMainCharacter::Reload);
		Input->BindAction(PickAction, ETriggerEvent::Triggered, this, &AMainCharacter::Pick);
	}

}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	if (!bIsInvincible)
	{
		ChangeHealth(Health - DamageAmount);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s has %f amount of health left. Instigator controller: %s"), *GetName(), Health, *(EventInstigator->GetName()));

	if (CanPlayShotAnim)
	{
		CanPlayShotAnim = false;
		OnBeingShot();
	}
	
	return DamageAmount;
}

float AMainCharacter::ChangeHealth(const float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	
	PlayerController->UpdateHealthPercentage(Health/MaxHealth);
	return Health;
}

void AMainCharacter::GetAmmo(const int AdditionalAmmo)
{
	Weapon->AddAmmo(AdditionalAmmo);
	
	PlayerController->UpdateAmmoInMagCount(Weapon->GetMagazineAmmoCount());
	PlayerController->UpdateUnequippedAmmoCount(Weapon->GetUnequippedAmmoCount());
}

void AMainCharacter::MakeInvincibleFor(float Seconds)
{
	
	FTimerHandle InvincibilityTimerHandle;
	bIsInvincible = true;
	GetWorld()->GetTimerManager().SetTimer(
		InvincibilityTimerHandle,
		this,
		&AMainCharacter::OnInvincibilityFinished,
		Seconds,
		false);
}

void AMainCharacter::OnInvincibilityFinished()
{
	bIsInvincible = false;
}

void AMainCharacter::Heal(const float AdditionalHealth)
{
	if (Health < MaxHealth && AdditionalHealth > 0)
	{
		ChangeHealth(Health + AdditionalHealth);
	}
}

void AMainCharacter::TeleportToLocation(FVector Location)
{
	SetActorLocation(Location);
}

