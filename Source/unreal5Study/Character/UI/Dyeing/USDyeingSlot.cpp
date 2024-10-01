// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../../USCharacterBase.h"
#include "USDyeingData.h"
#include "NativeGameplayTags.h"
#include "../../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_DyeingSlotUpdate_Message, "UI.Message.DyeingSlotUpdate");

void UUSDyeingSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.RegisterListener(TAG_DyeingSlotUpdate_Message, this, &ThisClass::ResponseMessage);

	ImageArray.Add(Image_0);
	ImageArray.Add(Image_1);
	ImageArray.Add(Image_2);
	ImageArray.Add(Image_3);
	ImageArray.Add(Image_4);
}

void UUSDyeingSlot::ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload)
{
	if (ItemData.ItemName != Payload.ItemData.ItemName)
		return;

	SetData(Payload.ItemData);
}

void UUSDyeingSlot::SetData(FUSItemData _ItemData)
{
	ItemData = _ItemData;
	ItemImage->SetBrushFromTexture(ItemData.ItemIcon);
	ItemName->SetText(FText::FromString(ItemData.ItemName));

	for (uint8 i = 0; i < static_cast<uint8>(EItemColorParts::MAX); i++)
	{
		ImageArray[i]->SetColorAndOpacity(ItemData.GetChangeColor(i));
	}
}

const FUSItemData UUSDyeingSlot::GetItemData()
{
	return ItemData;
}

void UUSDyeingSlot::OnModularPartsIconButtonClicked()
{
	AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
	if (CharacterOwner == nullptr)
		return;

	CharacterOwner->ModularCharacterComponent->ChangeParts(ItemData);

	FDyeingMessageData Message;
	Message.Verb = TAG_DyeingPanel_Message;
	Message.PartsType = static_cast<int8>(ItemData.ItemCategory);
	Message.ItemData = ItemData;
	Message.MessageType = 0;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSystem.BroadcastMessage(Message.Verb, Message);
}

void UUSDyeingSlot::SetColorImage()
{
}
