// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USDyeingSlot.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingSlot : public UUSUserWidget
{
	GENERATED_BODY()
	
public:

	void SetData(struct FModularCharacterRaw ModularRaw);
	FModularCharacterRaw GetModular();
	UFUNCTION(BlueprintCallable)
	void OnModularPartsIconButtonClicked();
protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ItemName;

	FModularCharacterRaw ModularRawData;
};
