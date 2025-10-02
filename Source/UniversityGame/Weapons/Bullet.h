// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

class USphereComponent;

UCLASS()
class UNIVERSITYGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category=Collision)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category=Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category=Decal)
	UMaterialInterface* DecalMaterial;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnFired(const FVector& Direction) const;
	
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
