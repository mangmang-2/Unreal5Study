// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USPreviewMain.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSPreviewMain : public UUSUserWidget
{
	GENERATED_BODY()

public:
	UUSPreviewMain(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPreviewChange(struct FModularCharacterRaw ModularRaw);

	UFUNCTION()
    void OnApplyButtonClicked(); 
protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UUSPreviewSlotGrid> WBP_SlotGrid;

	UPROPERTY()
	TMap<int8, struct FModularCharacterRaw> UsePreviewModular;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> Button_Apply;
};
