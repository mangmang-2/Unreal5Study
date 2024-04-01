// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterial> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UMiniViewComponent> MiniView;
};
