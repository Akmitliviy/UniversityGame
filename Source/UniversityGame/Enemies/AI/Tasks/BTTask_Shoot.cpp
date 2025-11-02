// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"

#include "UniversityGame/Enemies/AI/SoldierAIController.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASoldierAIController* SoldierAIController = Cast<ASoldierAIController>(OwnerComp.GetAIOwner()); SoldierAIController != nullptr)
	{
		SoldierAIController->StartShooting();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}