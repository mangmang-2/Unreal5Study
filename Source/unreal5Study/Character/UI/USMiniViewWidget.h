// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../USPlayer.h"
#include "USMiniViewWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSMiniViewWidget : public UUSUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UTextureRenderTarget2D> RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UMaterial> Material;


public:
	UUSMiniViewWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void SetSceneCaptureComponent(class USceneCaptureComponent2D* SceneCaptureComponent);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	EViewType ViewType = EViewType::None;	
};
