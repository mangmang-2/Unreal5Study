// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/USMiniViewWidget.h"
#include "UMG/Public/Components/Image.h"
#include "../USPlayer.h"
#include "../../MiniView/MiniViewComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

UUSMiniViewWidget::UUSMiniViewWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUSMiniViewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AUSPlayer* Player = Cast<AUSPlayer>(GetOwningPlayerPawn());
	MiniView = NewObject<UMiniViewComponent>(this, UMiniViewComponent::StaticClass());
	if (MiniView && Player)
	{
		// 카메라를 붙였지만 스프링암에 붙은 소켓을 기준으로 해도 동일함
		// 스프링 암을 여러게 만들고 카메라만 이동하는 형태라면 그 스프링암을 이용해서 동일한 뷰를 볼수있음
		MiniView->SetComponent(Player->GetCameraComponent());
	}
}

void UUSMiniViewWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}