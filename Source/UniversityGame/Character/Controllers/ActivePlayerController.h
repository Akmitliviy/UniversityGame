// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "ActivePlayerController.generated.h"

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

public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	
	void ShowCrosshair();

	void HideCrosshair() const;

private:

	void CreateCrosshair();	
};
