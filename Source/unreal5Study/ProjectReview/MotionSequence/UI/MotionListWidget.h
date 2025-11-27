// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MotionListWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UMotionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void OnMainWidgetMoved(FVector2D Pos, FVector2D Size);
protected:
};
