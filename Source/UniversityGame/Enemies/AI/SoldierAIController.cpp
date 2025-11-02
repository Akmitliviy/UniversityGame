// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
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

	if(AEnemy* Enemy = Cast<AEnemy>(InPawn); Enemy != nullptr)
	{
		ControlledEnemy = Enemy;
		if(Enemy->GetBehaviorTree()->BlackboardAsset != nullptr)
		{
			BlackboardComponent->InitializeBlackboard(*Enemy->GetBehaviorTree()->BlackboardAsset);
			
			TargetPoints = Enemy->GetTargetPoints();
			if (TargetPoints.Num() > 0)
			{
				CurrentTargetPointIndex = 0;
				BlackboardComponent->SetValueAsVector(FName("TargetPoint"), TargetPoints[CurrentTargetPointIndex]->GetActorLocation());
			}
			BehaviorTreeComponent->StartTree(*Enemy->GetBehaviorTree());
		}
	}
}

void ASoldierAIController::OnPerceptionUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if(AMainCharacter* Player = Cast<AMainCharacter>(UpdateInfo.Target))
	{
		if(UpdateInfo.Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(FName("Player"), Player);
		}else
		{
			BlackboardComponent->SetValueAsObject(FName("Player"), nullptr);
		}
	}
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

void ASoldierAIController::SetIsDying(bool IsDying)
{
	BlackboardComponent->SetValueAsBool(FName("IsDying"), IsDying);
}

void ASoldierAIController::GoToNextTargetPoint()
{
	CurrentTargetPointIndex = TargetPoints.IsValidIndex(CurrentTargetPointIndex + 1) ? CurrentTargetPointIndex + 1 : 0;
	BlackboardComponent->SetValueAsVector(FName("TargetPoint"), TargetPoints[CurrentTargetPointIndex]->GetActorLocation());
}

void ASoldierAIController::StartShooting()
{
	if (ControlledEnemy != nullptr)
	{
		
		ControlledEnemy->Fire();
	}
}

void ASoldierAIController::SetScoped(const bool IsScoped)
{
	if (ControlledEnemy != nullptr)
	{
		ControlledEnemy->SetIsScoped(IsScoped);
	}
}

bool ASoldierAIController::RotateTowards(const FName Target)
{
	if (const AMainCharacter* Player = Cast<AMainCharacter>(BlackboardComponent->GetValueAsObject(Target));
		Player != nullptr && ControlledEnemy != nullptr)
	{
		const FVector TargetLocation = Player->GetActorLocation();
		const FVector EnemyLocation = ControlledEnemy->GetActorLocation();
		const FRotator NewRotation = (TargetLocation - EnemyLocation).GetSafeNormal().Rotation();

		ControlledEnemy->SetActorRelativeRotation(FRotator(0.f, NewRotation.Yaw - 4.f, 0.f));
		return true;
	}
	return false;
}
