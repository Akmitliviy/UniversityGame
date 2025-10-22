// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_NextTargetPoint.h"

#include "UniversityGame/Enemies/AI/SoldierAIController.h"

EBTNodeResult::Type UBTTask_NextTargetPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASoldierAIController* SoldierAIController = Cast<ASoldierAIController>(OwnerComp.GetOwner()); SoldierAIController != nullptr)
	{
		SoldierAIController->GoToNextTargetPoint();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
