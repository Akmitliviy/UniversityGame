// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystemTypes.h"
#include "Blueprint/UserWidget.h"
#include "UniversityGame/HUD/PlayerAmmoWidget.h"
#include "UniversityGame/HUD/PlayerHealthbarWidget.h"


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
	CreateHealthBar();
	CreateAmmoWidget();
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

void AActivePlayerController::UpdateHealthPercentage(float Percentage)
{
	if (PlayerHealthBarWidget != nullptr)
	{
		PlayerHealthBarWidget->UpdateHealthPercentage(Percentage);
	}
}

void AActivePlayerController::UpdateGeneralAmmoCapacity(float Count)
{
	if (PlayerAmmoWidget != nullptr)
	{
		PlayerAmmoWidget->UpdateGeneralAmmoCapacity(Count);
	}
}

void AActivePlayerController::UpdateUnequippedAmmoCount(float Count)
{
	if (PlayerAmmoWidget != nullptr)
	{
		PlayerAmmoWidget->UpdateUnequippedAmmoCount(Count);
	}
}

void AActivePlayerController::UpdateMagCapacity(float Count)
{
	if (PlayerAmmoWidget != nullptr)
	{
		PlayerAmmoWidget->UpdateMagCapacity(Count);
	}
}

void AActivePlayerController::UpdateAmmoInMagCount(float Count)
{
	if (PlayerAmmoWidget != nullptr)
	{
		PlayerAmmoWidget->UpdateAmmoInMagCount(Count);
	}
}

void AActivePlayerController::CreateHealthBar()
{
	
	if (PlayerHealthBarClass != nullptr)
	{
		if (PlayerHealthBarWidget = CreateWidget<UPlayerHealthbarWidget>(this, PlayerHealthBarClass, TEXT("PlayerHealthBar")); PlayerHealthBarWidget != nullptr)
		{
			PlayerHealthBarWidget->AddToViewport();
		}
	}
}

void AActivePlayerController::CreateAmmoWidget()
{
	
	if (PlayerAmmoClass != nullptr)
	{
		if (PlayerAmmoWidget = CreateWidget<UPlayerAmmoWidget>(this, PlayerAmmoClass, TEXT("PlayerAmmoWidget")); PlayerAmmoWidget != nullptr)
		{
			PlayerAmmoWidget->AddToViewport();
		}
	}
}
