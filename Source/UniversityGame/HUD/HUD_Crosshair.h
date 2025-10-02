// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYGAME_API AHUD_Crosshair : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void DrawHUD() override;

private:
	UPROPERTY(EditDefaultsOnly, Category=Texture)
	UTexture2D* Texture;

	UPROPERTY()
	bool bShowCrosshair = true;

public:
	void SetShowCrosshair(const bool ShowCrosshair);
};
