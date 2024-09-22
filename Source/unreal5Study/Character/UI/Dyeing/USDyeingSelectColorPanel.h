// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingSelectColorPanel.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingSelectColorPanel : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	UUSDyeingSelectColorPanel(const FObjectInitializer& ObjectInitializer);
	virtual void ResponseMessage(int32 MessageType, UWidgetMessage* WidgetMessage);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUSDyeingColorDetail> WBP_DyeingColorDetail_Origin;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUSDyeingColorDetail> WBP_DyeingColorDetail_Select;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUSDyeingColorDetail> WBP_DyeingColorDetail_Random;
	
};
