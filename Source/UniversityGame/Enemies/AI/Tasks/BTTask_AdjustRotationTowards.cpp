// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AdjustRotationTowards.h"
#include "UniversityGame/Enemies/AI/SoldierAIController.h"

UBTTask_AdjustRotationTowards::UBTTask_AdjustRotationTowards()
{
	NodeName = "Adjust Rotation Towards";

	BlackboardKey.AddObjectFilter(this, BlackboardKey.SelectedKeyName, AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_AdjustRotationTowards::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASoldierAIController* SoldierAIController = Cast<ASoldierAIController>(OwnerComp.GetAIOwner()); SoldierAIController != nullptr)
	{
		if (SoldierAIController->RotateTowards(BlackboardKey.SelectedKeyName))
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
