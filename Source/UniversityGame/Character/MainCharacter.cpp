// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	JumpButtonDown = false;
	CrouchButtonDown = false;
	ScopeButtonDown = false;
	CanMove = true;
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (!CanMove) return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector MovementMath = FVector(MovementVector.Y, MovementVector.X, 0.f);
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector Vector = UKismetMathLibrary::Normal(FRotationMatrix(YawRotation).TransformVector(MovementMath));
	const double SpeedModifier = CrouchButtonDown
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
	CrouchButtonDown = Value.Get<bool>();
}

void AMainCharacter::CustomJump(const FInputActionValue& Value)
{
	JumpButtonDown = Value.Get<bool>();
}

void AMainCharacter::Scope(const FInputActionValue& Value)
{
	ScopeButtonDown = Value.Get<bool>();
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
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::CustomJump);
		Input->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::CustomCrouch);
		Input->BindAction(ScopeAction, ETriggerEvent::Triggered, this, &AMainCharacter::Scope);
	}

}

