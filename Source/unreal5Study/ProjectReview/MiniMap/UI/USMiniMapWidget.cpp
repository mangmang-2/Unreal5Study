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

	//TryBindRenderTarget();
}

void UUSMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// RT 연결이 늦게 되는 경우 대비(포제션 타이밍 등)
	//TryBindRenderTarget();

	//APawn* Pawn = GetOwningPlayerPawn();
	//if (Pawn == nullptr)
	//	return;

	//if (PlayerArrow == nullptr)
	//	return;

	//const float Yaw = Pawn->GetActorRotation().Yaw + ArrowYawOffset;
	//FWidgetTransform T = PlayerArrow->GetRenderTransform();
	//T.Angle = Yaw;
	//PlayerArrow->SetRenderTransform(T);
}

void UUSMiniMapWidget::TryBindRenderTarget()
{

	//APawn* Pawn = GetOwningPlayerPawn();
	//if (Pawn == nullptr) 
	//	return;

	//UUSMiniMapComponent* Comp = Pawn->FindComponentByClass<UUSMiniMapComponent>();
	//if (Comp == nullptr) 
	//	return;
	//if (UTextureRenderTarget2D* RT = Comp->GetRenderTarget())
	//{
	//	FSlateBrush Brush;
	//	Brush.SetResourceObject(RT);
	//	MinimapImage->SetBrush(Brush);
	//	//MinimapImage->SetBrushFromTexture(RT, true);
	//}
}
