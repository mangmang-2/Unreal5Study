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
	UUSMiniViewWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
	void SetComponent(class USceneComponent* InParent);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	EViewType ViewType = EViewType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Capture)
	TObjectPtr<class USceneCaptureComponent2D> sceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UTextureRenderTarget2D> renderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UMaterial> Material;
};
