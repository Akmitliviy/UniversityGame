// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthbarWidget.h"

void UPlayerHealthbarWidget::UpdateHealthPercentage(float Percentage)
{
	HealthPercentage = Percentage;
	OnHealthChanged();
}
