// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAmmoWidget.h"

void UPlayerAmmoWidget::UpdateGeneralAmmoCapacity(float Count)
{
	GeneralAmmoCapacity = Count;
	OnUpdateGeneralAmmoCapacity();
}

void UPlayerAmmoWidget::UpdateUnequippedAmmoCount(float Count)
{
	UnequippedAmmoCount = Count;
	OnUpdateUnequippedAmmoCount();
}

void UPlayerAmmoWidget::UpdateMagCapacity(float Count)
{
	MagCapacity = Count;
	OnUpdateMagCapacity();
}

void UPlayerAmmoWidget::UpdateAmmoInMagCount(float Count)
{
	AmmoInMagCount = Count;
	OnUpdateAmmoInMagCount();
}
