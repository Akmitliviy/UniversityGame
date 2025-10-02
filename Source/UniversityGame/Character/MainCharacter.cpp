// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputSubsystems.h"
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

	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	if(Weapon)
	{
		Weapon->SetActorEnableCollision(false);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("hand_r_weapon_socket"));
	}
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

	const FRotator CameraRotation = GetBaseAimRotation();
	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector CameraDirection = CameraComponent->GetForwardVector();

	const FVector MuzzleLocation = Weapon->GetMuzzleLocation();
	const FVector MuzzleDirection = Weapon->GetWeaponDirection();
	
	FVector LaunchDirection;
	if (ScopeButtonDown)
	{
		LaunchDirection = UKismetMathLibrary::Normal(MuzzleDirection);
		Weapon->Fire(CameraRotation, LaunchDirection);
	}else
	{
		
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;
		
		FHitResult RV_Hit;
		GetWorld()->LineTraceSingleByChannel(RV_Hit, CameraLocation, CameraLocation + CameraDirection * 10000.f, ECC_Visibility, RV_TraceParams);
		
		LaunchDirection = UKismetMathLibrary::Normal(RV_Hit.ImpactPoint - MuzzleLocation);
		Weapon->Fire(CameraRotation, LaunchDirection);
	}
}

void AMainCharacter::Reload(const FInputActionValue& Value)
{
	if (Weapon->CanReload())
	{
		CanShoot = false;
		Weapon->Reload();
		OnReload();
	}
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
	}

}

