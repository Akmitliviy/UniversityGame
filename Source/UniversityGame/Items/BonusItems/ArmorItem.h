// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorItem.generated.h"

class USphereComponent;

UCLASS()
class UNIVERSITYGAME_API AArmorItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category=Collision)
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float InvincibilityDuration = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemRotationSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemMaxAscendingHeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemAscendingSpeed = 0.1f;
public:	
	// Sets default values for this actor's properties
	AArmorItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	float CurrentAscendingRadianValue = 0.0f;
};
