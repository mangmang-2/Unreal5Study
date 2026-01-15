// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MotionMainWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMainWidgetMoved, FVector2D, Pos, FVector2D, Size);

UCLASS()
class UNREAL5STUDY_API UMotionMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMotionPresetWidget> MotionPresetWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UMotionPlayWidget> MotionPlayWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> SizeBox;
	
public:
	FVector2D GetViewSize();
	FVector2D GetViewPosition();

protected:
	UPROPERTY()
	bool bDragging = false;

	UPROPERTY()
	FVector2D DragStartMousePos;

	UPROPERTY()
	FVector2D DragStartWidgetPos;

	UPROPERTY()
	FVector2D StartViewportOffset;

	FVector2D CurrentPos;
	FVector2D PrevSize;
public:
	UPROPERTY(BlueprintAssignable)
    FOnMainWidgetMoved OnMainWidgetMoved;
public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void RefreshPostion();
};
