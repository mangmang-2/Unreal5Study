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
#include "AIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	if(Class == nullptr)
		return;
	auto IconWidget = CreateWidget<UUSMiniMapMarkerWidget>(GetWorld(), Class);
	if (IconWidget == nullptr)
		return;

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
	MoveToPlayer(Marker);
	DrawMoveLine(Marker);
}

void UUSMiniMapWidget::SpawnWorldPing(FVector WorldPos)
{
	if (PingActorClass == nullptr)
		return;

	GetWorld()->SpawnActor<AWorldPingActor>(PingActorClass,
			WorldPos + FVector(0, 0, 0),
			FRotator::ZeroRotator);
}

void UUSMiniMapWidget::MoveToPlayer(UUSMiniMapMarkerComponent* Marker)
{
	if (Marker == nullptr)
		return;

	APlayerController* PC = GetOwningPlayer();
	if (PC == nullptr)
		return;

	APawn* Pawn = PC->GetPawn();
	if (Pawn == nullptr)
		return;

	// AI Move To 사용
	AAIController* AIC = Cast<AAIController>(Pawn->GetController());
	if (AIC)
	{
		AIC->MoveToLocation(Marker->GetWorldLocation(), 50.f, true, true, false, true);
		return;
	}

	// AIController가 없으면 직접 이동 (단순 SetActorLocation or Navigate)
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys == nullptr)
		return;

	FNavLocation NavLocation;
	bool bFound = NavSys->ProjectPointToNavigation(
		Marker->GetWorldLocation(),
		NavLocation,
		FVector(200.f, 200.f, 200.f)
	);

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
		if (MoveComp)
		{
			// 이동 방향으로 자동 회전
			MoveComp->bOrientRotationToMovement = true;
			MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
		}

		// 컨트롤러 회전 사용 끄기 (이게 켜져 있으면 OrientRotation이 무시됨)
		Character->bUseControllerRotationYaw = false;

		if (bFound)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC, NavLocation.Location);
		}
	}
}

void UUSMiniMapWidget::DrawMoveLine(UUSMiniMapMarkerComponent* Marker)
{
	if (Marker == nullptr)
		return;

	// 기존 경로 초기화
	MoveLinePoints.Empty();
	TargetMarker = Marker;

	GetWorld()->GetTimerManager().SetTimer(
		MoveLineUpdateTimer,
		this,
		&UUSMiniMapWidget::UpdateMoveLine,
		0.1f,
		true
	);
}

void UUSMiniMapWidget::UpdateMoveLine()
{
	// 목적지 마커 유효성 체크
	if (!TargetMarker.IsValid())
	{
		ClearMoveLine();
		return;
	}

	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn == nullptr)
		return;

	MoveLinePoints.Empty();

	FVector PlayerWorld = Pawn->GetActorLocation();
	FVector2D PlayerUV = ConvertWorldToMiniMap(PlayerWorld);
	MoveLinePoints.Add(PlayerUV);

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		GetWorld(),
		PlayerWorld,
		TargetMarker->GetWorldLocation(),
		Pawn
	);

	if (NavPath && NavPath->IsValid())
	{
		for (const FVector& PathPoint : NavPath->PathPoints)
		{
			MoveLinePoints.Add(ConvertWorldToMiniMap(PathPoint));
		}

		// 목적지 근처에 도달하면 경로 제거
		float DistToTarget = FVector::Dist(PlayerWorld, TargetMarker->GetWorldLocation());
		if (DistToTarget < 100.f)
		{
			ClearMoveLine();
			return;
		}
	}
	else
	{
		// NavMesh 없으면 직선으로
		MoveLinePoints.Add(ConvertWorldToMiniMap(TargetMarker->GetWorldLocation()));
	}

	if (IconLayer)
	{
		IconLayer->InvalidateLayoutAndVolatility();
	}
}

void UUSMiniMapWidget::ClearMoveLine()
{
	MoveLinePoints.Empty();
	TargetMarker.Reset();
	GetWorld()->GetTimerManager().ClearTimer(MoveLineUpdateTimer);
}

int32 UUSMiniMapWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
	int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Layer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect,
		OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (MoveLinePoints.Num() >= 2)
	{
		FLinearColor LineColor = FLinearColor(0.f, 1.f, 0.4f, 0.85f);  // 초록
		float LineThickness = 5.f;

		for (int32 i = 0; i < MoveLinePoints.Num() - 1; ++i)
		{
			FVector2D Start = MoveLinePoints[i];
			FVector2D End = MoveLinePoints[i + 1];

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				Layer + 1,
				AllottedGeometry.ToPaintGeometry(),
				{ Start, End },
				ESlateDrawEffect::None,
				LineColor,
				true,
				LineThickness
			);
		}
	}

	return Layer;
}

void UUSMiniMapWidget::UpdateCharacterRotation()
{
	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn == nullptr)
	{
		return;
	}

	FVector Velocity = Pawn->GetVelocity();
	if (Velocity.SizeSquared() < 1.f)
		return;

	// 이동 방향으로 회전
	FRotator TargetRot = Velocity.Rotation();
	TargetRot.Pitch = 0.f;
	TargetRot.Roll = 0.f;

	FRotator CurrentRot = Pawn->GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, 0.05f, 10.f);

	Pawn->SetActorRotation(NewRot);
}