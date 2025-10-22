// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYGAME_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float HealthPercentage = 1.f;
public:
	UFUNCTION()
	void UpdateHealthPercentage(float Percentage);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged();
};
