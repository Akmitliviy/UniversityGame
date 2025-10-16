// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UniversityGame/Character/MainCharacter.h"
#include "UniversityGame/Enemies/Enemy.h"

ASoldierAIController::ASoldierAIController()
{
	CustomPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight->SightRadius = 1000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	
	CustomPerceptionComponent->ConfigureSense(*Sight);
	CustomPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("EnemyBehaviorTree"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("EnemyBlackBoard"));

	TeamId = FGenericTeamId(1);
	
}

void ASoldierAIController::BeginPlay()
{
	Super::BeginPlay();

	CustomPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ASoldierAIController::OnPerceptionUpdate);
}

void ASoldierAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(const AEnemy* Enemy = Cast<AEnemy>(InPawn))
	{
		if(Enemy->GetBehaviorTree()->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*Enemy->GetBehaviorTree()->BlackboardAsset);
			BlackboardComponent->SetValueAsVector(FName("TargetPointStart"), Enemy->GetTargetPointStart());
			BlackboardComponent->SetValueAsVector(FName("TargetPointEnd"), Enemy->GetTargetPointEnd());
			BehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree());
		}
	}
}

void ASoldierAIController::OnPerceptionUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	// if(AMainCharacter* Player = Cast<AMainCharacter>(UpdateInfo.Target))
	// {
	// 	BlackboardComponent->SetValueAsBool(FName("HasTarget"), UpdateInfo.Stimulus.WasSuccessfullySensed());
	// 	BlackboardComponent->SetValueAsObject(FName("MainCharacter"), Player);
	// }
}

ETeamAttitude::Type ASoldierAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if(const auto Player = Cast<AMainCharacter>(&Other); Player != nullptr)
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}

void ASoldierAIController::SetIsReloading(bool IsReloading)
{
	BlackboardComponent->SetValueAsBool(FName("IsReloading"), IsReloading);
}

void ASoldierAIController::SetIsHit(bool IsHit)
{
	BlackboardComponent->SetValueAsBool(FName("IsHit"), IsHit);
}
