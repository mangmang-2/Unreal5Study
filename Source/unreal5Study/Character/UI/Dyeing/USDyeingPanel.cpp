// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingPanel.h"
#include "USDyeingData.h"
#include "../../USCharacterBase.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "USDyeingSlot.h"

UUSDyeingPanel::UUSDyeingPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    WidgetID = EWidgetID::DyeingPanel;
}

void UUSDyeingPanel::NativeConstruct()
{
    Super::NativeConstruct();
    SelectDyeingSlot->SetVisibility(ESlateVisibility::Collapsed);
}

void UUSDyeingPanel::ResponseMessage(int32 MessageType, UWidgetMessage* WidgetMessage)
{
    UDyeingMessage* DyeingMessage = Cast<UDyeingMessage>(WidgetMessage);
    switch (MessageType)
    {
    case 0: // ºÎÀ§ : Àå°©, ¸ðÀÚ, ½Å¹ß..
        PartsType = DyeingMessage->PartsType;
        SelectDyeingSlot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        SelectDyeingSlot->SetData(DyeingMessage->ModularData);
        break;
    case 1: // color ÆÄÃ÷ : base, metal..
        ColorParts = DyeingMessage->ColorParts;
        break;
    case 2: // color 
        Color = DyeingMessage->Color;
        break;
    default:
        break;
    }
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

    CharacterOwner->ModularCharacterComponent->ChangePartsColor(PartsType, ColorParts, Color.ReinterpretAsLinear());
}

