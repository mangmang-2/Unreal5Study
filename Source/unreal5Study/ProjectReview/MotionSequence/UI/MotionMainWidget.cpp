// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MotionSequence/UI/MotionMainWidget.h"
#include "MotionMainWidget.h"
#include "MotionPlayWidget.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanelSlot.h"
//#include "../MotionSeqenceSubsystem.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "Engine/GameViewportClient.h"
#include "Widgets/SViewport.h"
#include "Blueprint/WidgetLayoutLibrary.h"

/*
NativeOnInitialized	위젯 생성 1번	버튼 바인딩, 초기 레퍼런스 설정
NativePreConstruct	에디터 프리뷰 + Construct 전에	디자인 시간 값 갱신용
NativeConstruct	AddToViewport 할 때마다	UI 리프레시, 데이터 그리기
NativeTick	매 프레임	필요할 때만
NativeDestruct	RemoveFromParent 시점	클린업
*/

/*
UE5에서 UMG 위젯은 보통 뷰포트 기준 좌표를 씀.
하지만 GetCursorPos() 는 모니터 기준 좌표를 줌.
그래서 전체화면일 때는 좌표가 잘 맞는데 에디터 상으로는 잘맞지 않음
에디터 뷰의 시작점을 0,0으로 하면 잘맞는것을 알수있음
그래서 실제 에디터의 좌표 값을 밀어줘서 에디터 상에서도 잘작동하고 전체화면에서도 잘 작동하도록 수정
*/
FVector2D GetViewportOffset()
{
	if (GEngine && GEngine->GameViewport)
	{
		UGameViewportClient* ViewportClient = GEngine->GameViewport;
		TSharedPtr<SViewport> SViewportWidget = ViewportClient->GetGameViewportWidget();

		if (SViewportWidget.IsValid())
		{
			return SViewportWidget->GetCachedGeometry().GetAbsolutePosition();
		}
	}

	return FVector2D::ZeroVector;
}

void UMotionMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (UMotionSeqenceSubsystem* SeqenceSubsystem = UGameInstance::GetSubsystem<UMotionSeqenceSubsystem>(GetGameInstance()))
	{
		SeqenceSubsystem->GetMotionList();
	}*/

	if(MotionPlayWidget)
		MotionPlayWidget->ParentMainWidget = this;

	// 초기값 세팅
	CurrentPos = {200,200};
	SetPositionInViewport(CurrentPos, true);
}

FVector2D UMotionMainWidget::GetViewSize()
{
	float UDPI = UWidgetLayoutLibrary::GetViewportScale(this);

	FGeometry Geo = GetCachedGeometry();
	FVector2D RealSize = Geo.GetLocalSize();
	return RealSize * UDPI;
}

FVector2D UMotionMainWidget::GetViewPosition()
{
	CurrentPos = GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector) - GetViewportOffset();
	return CurrentPos;
}

FReply UMotionMainWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = true;

		// OS 커서 절대좌표
		POINT P;
		GetCursorPos(&P);
		DragStartMousePos = FVector2D(P.x, P.y);

		// 위젯의 화면 위치
		DragStartWidgetPos = GetCachedGeometry().LocalToAbsolute(FVector2D::ZeroVector);
		OnMainWidgetMoved.Broadcast(CurrentPos, GetViewSize());
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply UMotionMainWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bDragging = false;
	return FReply::Handled();
}

void UMotionMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bDragging)
	{
		POINT P;
		GetCursorPos(&P);

		FVector2D NowMouse(P.x, P.y);
		FVector2D Delta = NowMouse - DragStartMousePos;

		
		CurrentPos = DragStartWidgetPos + Delta - GetViewportOffset();

		SetPositionInViewport(CurrentPos, true);
		OnMainWidgetMoved.Broadcast(CurrentPos, GetViewSize());
	}

	FVector2D NewSize = MyGeometry.GetLocalSize();

	if (!NewSize.Equals(PrevSize))
	{
		// 화면 크기 변경 감지
		OnMainWidgetMoved.Broadcast(CurrentPos, NewSize);
		PrevSize = NewSize;
	}
}

void UMotionMainWidget::RefreshPostion()
{
	OnMainWidgetMoved.Broadcast(GetViewPosition(), GetViewSize());
}
