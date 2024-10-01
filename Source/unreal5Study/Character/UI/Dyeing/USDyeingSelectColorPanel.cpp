// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingSelectColorPanel.h"
#include "USDyeingData.h"
#include "USDyeingColorDetail.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DyeingSelectColorPanel_Message, "UI.Message.DyeingSelectColorPanel");

void UUSDyeingSelectColorPanel::NativeConstruct()
{
    Super::NativeConstruct();

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    ListenerHandle = MessageSubsystem.RegisterListener(TAG_DyeingSelectColorPanel_Message, this, &ThisClass::ResponseMessage);
}

void UUSDyeingSelectColorPanel::NativeDestruct()
{
    Super::NativeDestruct();

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    MessageSubsystem.UnregisterListener(ListenerHandle);
}

UUSDyeingSelectColorPanel::UUSDyeingSelectColorPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}


void UUSDyeingSelectColorPanel::ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload)
{
	switch (Payload.MessageType)
    {
    case 0: // 원본
        SetColor(WBP_DyeingColorDetail_Origin, Payload.Color);
        break;
    case 1: // 선택한 색상
        SetColor(WBP_DyeingColorDetail_Select, Payload.Color);
        break;
    default:
        break;
    }
}

void UUSDyeingSelectColorPanel::SetColor(UUSDyeingColorDetail* Detail, FLinearColor Color)
{
    if (Detail == nullptr)
        return;
    
    Detail->SetColor(Color);

}
