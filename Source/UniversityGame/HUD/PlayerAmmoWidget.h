// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYGAME_API UPlayerAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float GeneralAmmoCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float UnequippedAmmoCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MagCapacity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AmmoInMagCount;

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateGeneralAmmoCapacity();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateUnequippedAmmoCount();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateMagCapacity();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateAmmoInMagCount();

public:
	void UpdateGeneralAmmoCapacity(float Count);
	void UpdateUnequippedAmmoCount(float Count);
	void UpdateMagCapacity(float Count);
	void UpdateAmmoInMagCount(float Count);
};
