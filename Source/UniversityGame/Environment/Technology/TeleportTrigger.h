// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportTrigger.generated.h"

class ATargetPoint;
class UBoxComponent;

UCLASS()
class UNIVERSITYGAME_API ATeleportTrigger : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category=Collision)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditInstanceOnly, Category=Teleportation)
	ATargetPoint* Destination;
	
public:	
	// Sets default values for this actor's properties
	ATeleportTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
