// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MotionListWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMotinListSelect, int32, Index);

UCLASS()
class UNREAL5STUDY_API UMotionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnMainWidgetMoved(FVector2D Pos, FVector2D Size);

	UFUNCTION()
	void OnClickMotionButton();

	void ClickMotionButton(int32 Index);

	UFUNCTION()
	void OnClickApplyButton();
protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox;
	

	UPROPERTY()
	TArray<TObjectPtr<class UButton>> MotionButtons;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> RowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button_Apply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Style")
	FButtonStyle NormalStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FButtonStyle SelectedStyle;

	int32 SelectIndex = -1;
public:
	FOnMotinListSelect OnMotinListSelect;
};
