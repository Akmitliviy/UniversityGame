// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBarWidget.h"

void UEnemyHealthBarWidget::UpdateHealthPercentage(float Percentage)
{
	HealthPercentage = Percentage;
	OnHealthChanged();
}
