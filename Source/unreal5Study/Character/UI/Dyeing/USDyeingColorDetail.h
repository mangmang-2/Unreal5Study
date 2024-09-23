// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingColorDetail.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingColorDetail : public UUSUserWidget
{
	GENERATED_BODY()

public:
	void InitText(FText InputColor);

	void SetColor(FLinearColor InputColor);

protected:

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> Color;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> Button;
};
