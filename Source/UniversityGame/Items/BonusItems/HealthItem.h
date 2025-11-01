// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthItem.generated.h"

class USphereComponent;

UCLASS()
class UNIVERSITYGAME_API AHealthItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category=Collision)
	USphereComponent* SphereComponent;

public:	
	// Sets default values for this actor's properties
	AHealthItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float HealthRecoveryAmount = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemRotationSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemMaxAscendingHeight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	float ItemAscendingSpeed = 0.1f;

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
