// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectReview/MiniMap/UI/USMiniMapWidget.h"
#include "USMiniMapWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Pawn.h"
#include "../USMiniMapComponent.h"
#include "Engine/TextureRenderTarget2D.h"

void UUSMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUSMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn == nullptr)
		return;

	if (PlayerArrow == nullptr)
		return;

	const float Yaw = Pawn->GetActorRotation().Yaw + ArrowYawOffset;
	FWidgetTransform T = PlayerArrow->GetRenderTransform();
	T.Angle = Yaw;
	PlayerArrow->SetRenderTransform(T);
}
