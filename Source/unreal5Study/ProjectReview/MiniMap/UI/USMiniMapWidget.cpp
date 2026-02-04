// Fill out your copyright notice in the Description page of Project Settings.
#include "ProjectReview/MiniMap/UI/USMiniMapWidget.h"
#include "USMiniMapWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Pawn.h"
#include "../Object/USMiniMapComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "../Mgr/USMiniMapGameInstanceSubsystem.h"
#include "../Object/USMiniMapMarkerComponent.h"
#include "Components/Widget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "USMiniMapMarkerWidget.h"
#include "../Actor/WorldPingActor.h"

void UUSMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn)
	{
		MiniMapComponent = Pawn->FindComponentByClass<UUSMiniMapComponent>();
	}

	if (UUSMiniMapGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSMiniMapGameInstanceSubsystem>(GetWorld()->GetGameInstance()))
	{
		for (auto& Entry : Subsystem->GetMarkers())
		{
			UUSMiniMapMarkerComponent* Marker = Entry.MarkerComp;
			if (Marker == nullptr)
				continue;

			CreateIcon(Marker);
		}
	}
}

void UUSMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IconLayer && MapSize.IsZero())
	{
		MapSize = IconLayer->GetCachedGeometry().GetLocalSize();
	}

	DrawMarkers();

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

	// 줌 속도
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

void UUSMiniMapWidget::DrawMarkers()
{
	for (auto [Marker, IconWidget] : MarkerIconMap)
	{
		if(IsValid(Marker) == false)
			continue;
		if (IsValid(IconWidget) == false)
			continue;

		if(IsInMiniMap(Marker->GetWorldLocation()))
		{
			FVector2D Pos = ConvertWorldToMiniMap(Marker->GetWorldLocation());

			if (UCanvasPanelSlot* CastSlot = Cast<UCanvasPanelSlot>(IconWidget->Slot))
			{
				if(IconWidget->IsVisible() == false)
					IconWidget->SetVisibility(ESlateVisibility::Visible);
				CastSlot->SetPosition(Pos);
			}
		}
		else
		{
			IconWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}

bool UUSMiniMapWidget::IsInMiniMap(const FVector& WorldPos) const
{
	if (MiniMapComponent == nullptr)
		return false;
	if (IconLayer == nullptr)
		return false;

	FVector Center = MiniMapComponent->GetPos();

	FVector Offset = WorldPos - Center;
	float HalfWidth = MiniMapComponent->OrthoWidth * 0.5f;

	float NormalizedX = (-Offset.X / HalfWidth) * 0.5f + 0.5f;
	float NormalizedY = (Offset.Y / HalfWidth) * 0.5f + 0.5f;

	NormalizedY = 1.f - NormalizedY;

	FVector2D MiniPos;
	MiniPos.X = NormalizedX * MapSize.X;
	MiniPos.Y = NormalizedY * MapSize.Y;
	
	const double PendingValue = 16.0;

	if (MiniPos.X <= PendingValue || MiniPos.X >= MapSize.X - PendingValue ||
		MiniPos.Y <= PendingValue || MiniPos.Y >= MapSize.Y - PendingValue)
	{
		return false;
	}

	return true;
}

FVector2D UUSMiniMapWidget::ConvertWorldToMiniMap(const FVector& WorldPos) const
{
	if(IconLayer == nullptr)
		return FVector2D::ZeroVector;

	if (MiniMapComponent == nullptr)
		return FVector2D::ZeroVector;

	FVector Offset = WorldPos - MiniMapComponent->GetPos();

	// OrthoWidth는 전체 폭 (즉 반지름은 절반)
	float HalfWidth = MiniMapComponent->OrthoWidth * 0.5f;

	float NormalizedX = (-Offset.X / HalfWidth) * 0.5f + 0.5f;
	float NormalizedY = (Offset.Y / HalfWidth) * 0.5f + 0.5f;

	NormalizedY = 1.f - NormalizedY;

	FVector2D MiniPos;
	MiniPos.X = NormalizedX * MapSize.X;
	MiniPos.Y = NormalizedY * MapSize.Y;

	return MiniPos;
}

void UUSMiniMapWidget::CreateIcon(UUSMiniMapMarkerComponent* Marker)
{
	if (MarkerIconMap.Contains(Marker))
		return;

	auto Class = Marker->MarkerIconClass.LoadSynchronous();
	auto IconWidget = CreateWidget<UUSMiniMapMarkerWidget>(GetWorld(), Class);

	IconWidget->Init(Marker);

	// 클릭 콜백 연결
	IconWidget->OnMarkerClicked.AddDynamic(this, &UUSMiniMapWidget::HandleMarkerClicked);

	IconLayer->AddChild(IconWidget);

	MarkerIconMap.Add(Marker, IconWidget);
}

void UUSMiniMapWidget::HandleMarkerClicked(UUSMiniMapMarkerComponent* Marker)
{
	if(Marker == nullptr)
		return;

	SpawnWorldPing(Marker->GetWorldLocation());
}

void UUSMiniMapWidget::SpawnWorldPing(FVector WorldPos)
{
	if (PingActorClass == nullptr)
		return;

	GetWorld()->SpawnActor<AWorldPingActor>(PingActorClass,
			WorldPos + FVector(0, 0, 0),
			FRotator::ZeroRotator);
}
