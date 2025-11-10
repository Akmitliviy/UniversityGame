// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Evade.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "UniversityGame/Enemies/AI/SoldierAIController.h"

UBTTask_Evade::UBTTask_Evade()
{
	NodeName = "Evade";
}

EBTNodeResult::Type UBTTask_Evade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASoldierAIController* SoldierAIController = Cast<ASoldierAIController>(OwnerComp.GetOwner()); SoldierAIController != nullptr)
	{
		const FVector EvasionLocation = SoldierAIController->CalculateEvasionLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("EvasionLocation"), EvasionLocation);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
