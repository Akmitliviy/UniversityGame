// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Crosshair.h"

#include "Engine/Canvas.h"

void AHUD_Crosshair::DrawHUD()
{
	Super::DrawHUD();

	if (Texture != nullptr && bShowCrosshair)
	{
		const FVector2D Center(Canvas->ClipX*0.5f, Canvas->ClipY*0.5f);

		const FVector2D DrawPosition(Center.X - Texture->GetSurfaceWidth() * 0.5f, Center.Y - Texture->GetSurfaceHeight() * 0.5f);

		FCanvasTileItem TileItem(DrawPosition, Texture->GetResource(), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void AHUD_Crosshair::SetShowCrosshair(const bool ShowCrosshair)
{
	this->bShowCrosshair = ShowCrosshair;
}
