// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "../../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USDyeingData.h"
#include "USDyeingSelectColorPanel.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingSelectColorPanel : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UUSDyeingSelectColorPanel(const FObjectInitializer& ObjectInitializer);
	virtual void ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload);
	void SetColor(class UUSDyeingColorDetail* Detail, FLinearColor Color);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUSDyeingColorDetail> WBP_DyeingColorDetail_Origin;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUSDyeingColorDetail> WBP_DyeingColorDetail_Select;
	

	FGameplayMessageListenerHandle ListenerHandle;
};
