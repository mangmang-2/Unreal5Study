// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USCostItem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCostItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetData(class UTexture2D* textureIcon, int32 costValue);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> cost;
};
