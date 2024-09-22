// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingPanel.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingPanel : public UUSUserWidget
{
	GENERATED_BODY()
public:
	UUSDyeingPanel(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void ResponseMessage(int32 MessageType, class UWidgetMessage* WidgetMessage);

	UFUNCTION(BlueprintCallable)
	void StartDyeing();

protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UUSDyeingSlot> SelectDyeingSlot;

protected:
	uint8 PartsType = -1;
	uint8 ColorParts = -1;
	FColor Color;
};
