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

	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn)
	{
		MiniMapComponent = Pawn->FindComponentByClass<UUSMiniMapComponent>();
	}
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

FReply UUSMiniMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	float WheelDelta = InMouseEvent.GetWheelDelta();

	// ÁÜ ¼Óµµ
	float ZoomAmount = WheelDelta * 500.0f;

	if (MiniMapComponent)
	{
		MiniMapComponent->SetZoom(ZoomAmount);
	}

	return FReply::Handled();
}

FReply UUSMiniMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = true;
		LastMousePos = InMouseEvent.GetScreenSpacePosition();

		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UUSMiniMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bDragging && MiniMapComponent)
	{
		FVector2D CurrentPos = InMouseEvent.GetScreenSpacePosition();
		FVector2D Delta = CurrentPos - LastMousePos;

		LastMousePos = CurrentPos;

		MiniMapComponent->AddPan(FVector2D(Delta.X, Delta.Y));

		return FReply::Handled();
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UUSMiniMapWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UUSMiniMapWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bDragging = false;
	Super::NativeOnMouseLeave(InMouseEvent);
}
