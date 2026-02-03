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
#include "../../../../../../../Source/Runtime/UMG/Public/Components/Widget.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "../../../../../../../Source/Runtime/UMG/Public/Components/CanvasPanel.h"

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
	if (UUSMiniMapGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSMiniMapGameInstanceSubsystem>(GetWorld()->GetGameInstance()))
	{
		for (auto& Entry : Subsystem->GetMarkers())
		{
			UUSMiniMapMarkerComponent* Marker = Entry.MarkerComp;
			if (Marker == nullptr)
				continue;

			if (Marker->IsVisible() == false) 
				continue;

			FVector WorldPos = Marker->GetWorldLocation();

			if (IsInMiniMap(WorldPos))
			{
				FVector2D Pos = ConvertWorldToMiniMap(Marker->GetWorldLocation());

				if (auto FoundIcon = MarkerIconMap.Find(Marker))
				{
					UImage* Icon = *FoundIcon;
					if (Icon == nullptr)
						continue;

					if (UCanvasPanelSlot* CanvasSlot =
						Cast<UCanvasPanelSlot>(Icon->Slot))
					{
						CanvasSlot->SetPosition(Pos);
					}
				}
				else
				{
					DrawIcon(Marker, Pos);
				}
			}
			else
			{
				if (auto FoundIcon = MarkerIconMap.Find(Marker))
				{
					UImage* Icon = *FoundIcon;
					if (Icon == nullptr)
						continue;

					if (IsValid(Icon))
					{
						Icon->RemoveFromParent();
					}

					MarkerIconMap.Remove(Marker);
				}
			}
		}
	}
}

bool UUSMiniMapWidget::IsInMiniMap(const FVector& WorldPos) const
{
	if (MiniMapComponent == nullptr)
		return false;
	if (IconLayer == nullptr)
		return false;

	FVector2D MapSize = IconLayer->GetCachedGeometry().GetLocalSize();

	FVector Center = MiniMapComponent->GetPos();

	FVector Offset = WorldPos - Center;
	float HalfWidth = MiniMapComponent->OrthoWidth * 0.5f;

	float NormalizedX = (-Offset.X / HalfWidth) * 0.5f + 0.5f;
	float NormalizedY = (Offset.Y / HalfWidth) * 0.5f + 0.5f;

	NormalizedY = 1.f - NormalizedY;

	FVector2D MiniPos;
	MiniPos.X = NormalizedX * MapSize.X;
	MiniPos.Y = NormalizedY * MapSize.Y;

	if (MiniPos.X <= 0 || MiniPos.X >= MapSize.X ||
		MiniPos.Y <= 0 || MiniPos.Y >= MapSize.Y)
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

	FVector2D MapSize = IconLayer->GetCachedGeometry().GetLocalSize();

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

void UUSMiniMapWidget::DrawIcon(UUSMiniMapMarkerComponent* Marker, FVector2D MiniPos)
{
	if (IconLayer == nullptr)
		return;

	if (Marker == nullptr)
		return;
	if (Marker->MarkerTexture == nullptr)
		return;

	UImage* Icon = NewObject<UImage>(this);
	if (Icon == nullptr)
		return;

	Icon->SetBrushFromTexture(Marker->MarkerTexture);

	UCanvasPanelSlot* CanvasSlot = IconLayer->AddChildToCanvas(Icon);
	if (CanvasSlot == nullptr)
		return;

	const FVector2D IconSize(20.f, 20.f);

	CanvasSlot->SetSize(IconSize);
	CanvasSlot->SetPosition(MiniPos);
	CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

	MarkerIconMap.Add(Marker, Icon);
}
