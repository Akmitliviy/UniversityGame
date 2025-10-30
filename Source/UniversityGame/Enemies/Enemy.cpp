// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyHealthBarWidget.h"
#include "AI/SoldierAIController.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UniversityGame/Weapons/BaseWeapon.h"

// Sets default values
AEnemy::AEnemy() :
HealthBarWidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"))}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(true);
	
	Health = MaxHealth;
	
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetupAttachment(RootComponent);
		HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		if (UEnemyHealthBarWidget* HealthBarWidget = Cast<UEnemyHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
			HealthBarWidget != nullptr)
		{
			HealthBarWidget->UpdateHealthPercentage(Health / MaxHealth);
		}
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	Weapon = GetWorld()->SpawnActor<ABaseWeapon>(
		WeaponClass,
		FVector(0.f, 0.f, 0.f),
		FRotator(0.f, 0.f, 0.f),
		SpawnParams);
	if(Weapon)
	{
		Weapon->SetActorEnableCollision(false);
		Weapon->SetInfiniteAmmo(true);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("hand_r_venra_socket"));
	}
}

void AEnemy::Fire()
{
	if (WeaponClass == nullptr) return;
	if (!CanShoot || IsDying) return;

	const FVector MuzzleDirection = Weapon->GetWeaponDirection();
	
	const FVector LaunchDirection = UKismetMathLibrary::Normal(MuzzleDirection);
	
	if (Weapon->ReadyToFire())
		Weapon->Fire(LaunchDirection.Rotation(), LaunchDirection);

	if (Weapon->IsMagEmpty())
		Reload();
	
}

void AEnemy::Reload()
{
	if (Weapon->CanReload())
	{
		
		CanShoot = false;
		
		if(const auto SoldierController = Cast<ASoldierAIController>(GetController()); SoldierController != nullptr)
		{
			SoldierController->SetIsReloading(true);
		}
		OnReload();

		Weapon->Reload();
		
	}
}

void AEnemy::OnReloadFinal()
{
	CanShoot = true;
	if(const auto SoldierController = Cast<ASoldierAIController>(GetController()); SoldierController != nullptr)
	{
		SoldierController->SetIsReloading(false);
	}
}

void AEnemy::OnDeathFinal()
{
	//DropWeapon();
	Destroy();
}

void AEnemy::OnBeingShotFinal()
{
	CanPlayShotAnim = true;
	if(const auto SoldierController = Cast<ASoldierAIController>(GetController()); SoldierController != nullptr)
	{
		SoldierController->SetIsHit(false);
	}
}

void AEnemy::DropWeapon()
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

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Fire();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (IsImmortal || IsDying) return 0;
	
	if (Health > 0)
	{
		Health -= DamageAmount;
		if (UEnemyHealthBarWidget* HealthBarWidget = Cast<UEnemyHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
			HealthBarWidget != nullptr)
		{
			HealthBarWidget->UpdateHealthPercentage(Health / MaxHealth);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s has %f amount of health left. Instigator controller: %s"), *GetName(), Health, *(EventInstigator->GetName()));
		if (CanPlayShotAnim)
		{
			CanPlayShotAnim = false;
			if(const auto SoldierController = Cast<ASoldierAIController>(GetController()); SoldierController != nullptr)
			{
				SoldierController->SetIsHit(true);
			}
			OnBeingShot();
		}
	}

	if (Health <= 0 && !IsDying)
	{
		IsDying = true;
		if(const auto SoldierController = Cast<ASoldierAIController>(GetController()); SoldierController != nullptr)
		{
			SoldierController->SetIsDying(true);
		}
		DropWeapon();
		OnDeath();
	}

	return DamageAmount;
	
}

UBehaviorTree* AEnemy::GetBehaviorTree() const
{
	return BehaviorTree;
}

TArray<ATargetPoint*>& AEnemy::GetTargetPoints()
{
	return TargetPoints;
}
