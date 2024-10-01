// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USDyeingData.h"
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
	virtual void NativeDestruct() override;
	virtual void ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload);

	void SetOriginColor();
	void SetSelectColor();

	UFUNCTION(BlueprintCallable)
	void StartDyeing();

	UFUNCTION(BlueprintCallable)
	void SaveDyeing();

	UFUNCTION(BlueprintCallable)
	void ReturnDyeing();
protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UUSDyeingSlot> SelectDyeingSlot;

protected:
	uint8 PartsType = -1;
	uint8 ColorParts = -1;
	FLinearColor Color;

	FGameplayMessageListenerHandle ListenerHandle;
};
