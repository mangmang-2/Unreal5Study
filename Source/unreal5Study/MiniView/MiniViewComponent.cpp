// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniView/MiniViewComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values for this component's properties
UMiniViewComponent::UMiniViewComponent()
{
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetRef(TEXT("/Game/Study/RenderTarget/RenderTarget.RenderTarget"));
	if (RenderTargetRef.Succeeded())
	{
		renderTarget = RenderTargetRef.Object;
	}

}

void UMiniViewComponent::SetComponent(USceneComponent* InParent)
{
	sceneCapture = NewObject<USceneCaptureComponent2D>(InParent, USceneCaptureComponent2D::StaticClass());
	sceneCapture->RegisterComponent();

	if (sceneCapture)
		sceneCapture->AttachToComponent(InParent, FAttachmentTransformRules::KeepRelativeTransform);

	//renderTarget = NewObject<UTextureRenderTarget2D>(this, UTextureRenderTarget2D::StaticClass());
	//renderTarget->InitAutoFormat(256, 256); // 예시 해상도

	sceneCapture->TextureTarget = renderTarget;

}

