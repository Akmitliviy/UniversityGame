// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetScoped.h"

#include "UniversityGame/Enemies/AI/SoldierAIController.h"

UBTTask_SetScoped::UBTTask_SetScoped()
{
	NodeName = "Set Scoped";
}

EBTNodeResult::Type UBTTask_SetScoped::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASoldierAIController* SoldierAIController = Cast<ASoldierAIController>(OwnerComp.GetAIOwner()); SoldierAIController != nullptr)
	{
		SoldierAIController->SetScoped(bIsScoped);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
