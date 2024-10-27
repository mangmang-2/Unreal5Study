// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USBuildItem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSBuildItem : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();
	void SetData(struct FUSResource* Resource);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TitleImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ResourceList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> Border_Color;
};
