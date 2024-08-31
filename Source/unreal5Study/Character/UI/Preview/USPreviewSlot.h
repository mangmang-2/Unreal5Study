// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USPreviewSlot.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSPreviewSlot : public UUSUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> ModularPartsIcon;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> ModularPartsIconButton;
	
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UEditableTextBox> EditableTextBox_PartsName;

	FModularCharacterRaw ModularCharacterRaw;

public:
	void SetData(struct FModularCharacterRaw ModularRaw);


	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnModularPartsIconButtonClicked();
};
