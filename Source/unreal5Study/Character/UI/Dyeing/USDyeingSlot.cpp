// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../USCharacterBase.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "USDyeingData.h"
#include "NativeGameplayTags.h"
#include "../../../Lyra/GameFramework/GameplayMessageSubsystem.h"

//UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_DyeingPanel_Message, "UI.Message.DyeingPanel");


void UUSDyeingSlot::SetData(FModularCharacterRaw ModularRaw)
{
	ModularRawData = ModularRaw;
	ItemImage->SetBrushFromTexture(ModularRaw.ModularIcon);
	ItemName->SetText(FText::FromString(ModularRaw.ModularName));
}

FModularCharacterRaw UUSDyeingSlot::GetModular()
{
	return ModularRawData;
}

void UUSDyeingSlot::OnModularPartsIconButtonClicked()
{
	AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
	if (CharacterOwner == nullptr)
		return;

	CharacterOwner->ModularCharacterComponent->ChangeParts(ModularRawData);

	FDyeingMessageData Message;
	Message.Verb = TAG_DyeingPanel_Message;
	Message.PartsType = static_cast<int8>(ModularRawData.ModularCategory);
	Message.ModularData = ModularRawData;
	Message.MessageType = 0;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(Message.Verb, Message);
}
