// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"


class UEnhancedInputLocalPlayerSubsystem;

void AActivePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}
	}
}

void AActivePlayerController::BeginPlay()
{
	Super::BeginPlay();
	CreateCrosshair();
}

void AActivePlayerController::CreateCrosshair()
{

	if (CrosshairClass != nullptr)
	{
		if (CrosshairWidget = CreateWidget<UUserWidget>(this, CrosshairClass, TEXT("Crosshair")); CrosshairWidget != nullptr)
		{
			CrosshairWidget->AddToViewport();
		}
	}
}

void AActivePlayerController::ShowCrosshair()
{
	if (CrosshairWidget == nullptr)
	{
		CreateCrosshair();
	}else if (!CrosshairWidget->IsInViewport())
	{
		CrosshairWidget->AddToViewport();
	}
}

void AActivePlayerController::HideCrosshair() const
{
	if (CrosshairWidget != nullptr && CrosshairWidget->IsInViewport())
	{
		CrosshairWidget->RemoveFromParent();
	}
}
