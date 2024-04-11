// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/USMiniViewWidget.h"
#include "UMG/Public/Components/Image.h"

#include "GameFramework/SpringArmComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

UUSMiniViewWidget::UUSMiniViewWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UUSMiniViewWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AUSPlayer* Player = Cast<AUSPlayer>(GetOwningPlayerPawn());

	if (Player && ViewType != EViewType::None)
	{
		SetSceneCaptureComponent(Player->GetSceneCaptureComponent(ViewType));
	}
		
	UImage* Image = Cast<UImage>(GetWidgetFromName(TEXT("Image")));
	if (Image && Material)
	{
		Image->SetBrushFromMaterial(Material);
	}
}

//void UUSMiniViewWidget::SetComponent(USceneComponent* InParent)
//{
//	sceneCapture = NewObject<USceneCaptureComponent2D>(InParent, USceneCaptureComponent2D::StaticClass());
//	sceneCapture->RegisterComponent();
//
//	if (sceneCapture)
//		sceneCapture->AttachToComponent(InParent, FAttachmentTransformRules::KeepRelativeTransform);
//
//	sceneCapture->TextureTarget = RenderTarget;
//}

void UUSMiniViewWidget::SetSceneCaptureComponent(USceneCaptureComponent2D* SceneCaptureComponent)
{
	if (SceneCaptureComponent == nullptr)
		return;
	
	SceneCaptureComponent->TextureTarget = RenderTarget;
}
