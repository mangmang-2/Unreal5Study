// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USMiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> MinimapImage = nullptr;

	//UPROPERTY(meta=(BindWidget))
	//TObjectPtr<class UImage> PlayerArrow = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArrowYawOffset = 0.0f;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	void TryBindRenderTarget();
};
