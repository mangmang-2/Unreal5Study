// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingPanel.h"
#include "USDyeingData.h"
#include "../../USCharacterBase.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "USDyeingSlot.h"
#include "NativeGameplayTags.h"
#include "../../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USDyeingData.h"
#include "Item/USItemData.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DyeingPanel_Message, "UI.Message.DyeingPanel");
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Inventory_Update_Color_Message);

UUSDyeingPanel::UUSDyeingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUSDyeingPanel::NativeConstruct()
{
    Super::NativeConstruct();
    SelectDyeingSlot->SetVisibility(ESlateVisibility::Collapsed);

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    ListenerHandle = MessageSubsystem.RegisterListener(TAG_DyeingPanel_Message, this, &ThisClass::ResponseMessage);
}

void UUSDyeingPanel::NativeDestruct()
{
    Super::NativeDestruct();

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    MessageSubsystem.UnregisterListener(ListenerHandle);
}

void UUSDyeingPanel::ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload)
{
    switch (Payload.MessageType)
    {
    case 0: // ºÎÀ§ : Àå°©, ¸ðÀÚ, ½Å¹ß..
        PartsType = Payload.PartsType;
        SelectDyeingSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        SelectDyeingSlot->SetData(Payload.ItemData);
        SetOriginColor();
        break;
    case 1: // color ÆÄÃ÷ : base, metal..
        ColorParts = Payload.ColorParts;
        SetOriginColor();
        break;
    case 2: // color 
        Color = Payload.Color;
        SetSelectColor();
        break;
    default:
        break;
    }
}

void UUSDyeingPanel::SetOriginColor()
{
    FUSItemData Data = SelectDyeingSlot->GetItemData();
    FLinearColor LinearColor = Data.GetChangeColor(ColorParts);

    FDyeingMessageData Message;
    Message.Verb = TAG_DyeingSelectColorPanel_Message;
    Message.Color = LinearColor;
    Message.MessageType = 0;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(Message.Verb, Message);
}

void UUSDyeingPanel::SetSelectColor()
{
    FDyeingMessageData Message;
    Message.Verb = TAG_DyeingSelectColorPanel_Message;
    Message.Color = Color;
    Message.MessageType = 1;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(Message.Verb, Message);
}

void UUSDyeingPanel::StartDyeing()
{
    if (PartsType < 0)
        return;

    if (ColorParts < 0)
        return;

    AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
    if (CharacterOwner == nullptr)
        return;

    CharacterOwner->ModularCharacterComponent->ChangePartsColor(PartsType, ColorParts, Color);

    FUSItemData Data = SelectDyeingSlot->GetItemData();
    Data.SetChangeColor(ColorParts, Color);
    SelectDyeingSlot->SetData(Data);

    SetOriginColor();
    SendInven();
    SendSlot();
}

void UUSDyeingPanel::ReturnDyeing()
{
    if (PartsType < 0)
        return;

    if (ColorParts < 0)
        return;

    AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
    if (CharacterOwner == nullptr)
        return;

    CharacterOwner->ModularCharacterComponent->InitPartsColor(PartsType, ColorParts);
    SetOriginColor();
}

void UUSDyeingPanel::SendInven()
{
	FDyeingMessageData Message;
	Message.Verb = TAG_Inventory_Update_Color_Message;
	Message.ItemData = SelectDyeingSlot->GetItemData();
    Message.MessageType = 0;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(Message.Verb, Message);
}

void UUSDyeingPanel::SendSlot()
{
    FDyeingMessageData Message;
    Message.Verb = TAG_DyeingSlotUpdate_Message;
    Message.ItemData = SelectDyeingSlot->GetItemData();
    Message.MessageType = 0;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(Message.Verb, Message);
}

