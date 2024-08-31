// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USPreviewSlotGrid.generated.h"

enum class EModularCharacterType : uint8;
/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSPreviewSlotGrid : public UUSUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

	void Build(EModularCharacterType eCategory);
	void CreateSlotWidget(int32 InRow, int32 InColumn, struct FModularCharacterRaw ModularRaw);

	UFUNCTION()
	void OnCategorySelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UUniformGridPanel> GridPanel;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UComboBoxString> ComboBoxString_Category;
};
