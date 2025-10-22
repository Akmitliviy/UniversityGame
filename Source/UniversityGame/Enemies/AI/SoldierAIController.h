// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SoldierAIController.generated.h"

class ATargetPoint;
struct FActorPerceptionUpdateInfo;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class UNIVERSITYGAME_API ASoldierAIController : public AAIController
{
	GENERATED_BODY()

	FGenericTeamId TeamId;

private:
	UPROPERTY()
	TArray<ATargetPoint*> TargetPoints = TArray<ATargetPoint*>();

	UPROPERTY()
	int CurrentTargetPointIndex;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAIPerceptionComponent* CustomPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAISenseConfig_Sight* Sight;

public:
	ASoldierAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnPerceptionUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);

	virtual FGenericTeamId GetGenericTeamId() const override{return TeamId;}

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void SetIsReloading(bool IsReloading);

	UFUNCTION()
	void SetIsHit(bool IsHit);

	UFUNCTION()
	void SetIsDying(bool IsDying);

	UFUNCTION()
	void GoToNextTargetPoint();
};
