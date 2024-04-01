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
		//Player->sceneCapture->TextureTarget = MiniView->renderTarget;
		MiniView->SetComponent(Player->GetCameraComponent());
	}
}

void UUSMiniViewWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}