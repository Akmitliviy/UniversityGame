// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "ActivePlayerController.generated.h"

class UPlayerAmmoWidget;
class UPlayerHealthbarWidget;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UNIVERSITYGAME_API AActivePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerHealthbarWidget> PlayerHealthBarClass;

	UPROPERTY()
	UPlayerHealthbarWidget* PlayerHealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UPlayerAmmoWidget> PlayerAmmoClass;

	UPROPERTY()
	UPlayerAmmoWidget* PlayerAmmoWidget;

public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	
	void ShowCrosshair();

	void HideCrosshair() const;

	void UpdateHealthPercentage(float Percentage);

	
	void UpdateGeneralAmmoCapacity(float Count);
	void UpdateUnequippedAmmoCount(float Count);
	void UpdateMagCapacity(float Count);
	void UpdateAmmoInMagCount(float Count);

private:

	void CreateHealthBar();
	void CreateAmmoWidget();
	void CreateCrosshair();	
};
