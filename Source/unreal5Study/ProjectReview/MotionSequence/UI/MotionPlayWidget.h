// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MotionPlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UMotionPlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> UniformGridPanel;

	UPROPERTY()
	TArray<TObjectPtr<class UButton>> MotionButtons;

	UFUNCTION()
	void OnClickMotionButton();

	void ClickMotionButton(int32 Index);

	void ToggleListView();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Motion")
	TSubclassOf<class UMotionListWidget> MotionListWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMotionListWidget> MotionListWidget;

public:
	UPROPERTY()
	TObjectPtr<class UMotionMainWidget> ParentMainWidget;
};
