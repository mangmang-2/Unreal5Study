// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USBuild.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSBuild : public UUSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();
	void AddItem(struct FUSResource* Resource);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ItemList;
};
