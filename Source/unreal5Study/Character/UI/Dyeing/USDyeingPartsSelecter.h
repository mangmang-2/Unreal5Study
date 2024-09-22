// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingPartsSelecter.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingPartsSelecter : public UUSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTabButtonClicked();

	void SelectButton(uint8 ButtonIndex);
public:

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UVerticalBox> VerticalBox;

	TMap<uint32, class UButton*> ButtonList;

	FButtonStyle SaveStyle;
};
