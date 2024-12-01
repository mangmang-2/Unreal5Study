// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USResourceUIItem.h"
#include "../Interactable/USInteractable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Cropout_UI_Message, "Cropout.UI.Message.ResourceValue");

void UUSResourceUIItem::NativeConstruct()
{
	Super::NativeConstruct();

	UDataTable* DataTablePointer = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Cropout/Spawn/DT_CostTable.DT_CostTable"));

	if (DataTablePointer != nullptr)
	{
		TArray<FUSCostIcon*> AllRows;
		DataTablePointer->GetAllRows(nullptr, AllRows);

		for (auto& Row : AllRows)
		{
			if (Row->CostType == ResourceType)
			{
				Image_Resource->SetBrushFromTexture(Row->UIIcon);
				break;
			}
		}
	}
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	ListenerHandle = MessageSubsystem.RegisterListener(TAG_Cropout_UI_Message, this, &ThisClass::ResponseMessage);

}

void UUSResourceUIItem::ResponseMessage(FGameplayTag Channel, const FCropoutResourceValueMessageData& Payload)
{
	if (Payload.ResourceType != ResourceType)
		return;

	FText TextValue = FText::FromString(FString::FromInt(Payload.Value));
	TextBlock_Resource->SetText(TextValue);
}
