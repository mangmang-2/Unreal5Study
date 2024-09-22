// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../USCharacterBase.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "USDyeingData.h"

void UUSDyeingSlot::SetData(FModularCharacterRaw ModularRaw)
{
	ModularRawData = ModularRaw;
	ItemImage->SetBrushFromTexture(ModularRaw.ModularIcon);
	ItemName->SetText(FText::FromString(ModularRaw.ModularName));
}

void UUSDyeingSlot::OnModularPartsIconButtonClicked()
{
	AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
	if (CharacterOwner == nullptr)
		return;

	CharacterOwner->ModularCharacterComponent->ChangeParts(ModularRawData);

	UDyeingMessage* WidgetMessage = NewObject<UDyeingMessage>();
	WidgetMessage->PartsType = static_cast<int8>(ModularRawData.ModularCategory);
	WidgetMessage->ModularData = ModularRawData;
	SendMessage(EWidgetID::DyeingPanel, 0, WidgetMessage);
}
