// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingItemList.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingItemList : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> ItemList;

public:
	void CreateSlotWidget(struct FModularCharacterRaw ModularRaw);

};
