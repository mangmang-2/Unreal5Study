// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingItemList.h"
#include "USDyeingSlot.h"
#include "Components/VerticalBox.h"
#include "Item/USItemGameInstanceSubsystem.h"


void UUSDyeingItemList::NativeConstruct()
{
	Super::NativeDestruct();

	SlotList.Empty();
	if (UUSItemGameInstanceSubsystem* ItemSubsystem = UGameInstance::GetSubsystem<UUSItemGameInstanceSubsystem>(GetGameInstance()))
	{
		int32 Row = 0, Column = 0;
		TArray<FUSItemData> ItemArray;
		ItemSubsystem->GetItemList(ItemArray);
		for (const auto& ItemData : ItemArray)
		{
			CreateSlotWidget(ItemData);
		}
	}
}

void UUSDyeingItemList::CreateSlotWidget(FUSItemData ItemRaw)
{
	TSubclassOf<UUSDyeingSlot> SlotClass = LoadClass<UUSDyeingSlot>(nullptr, TEXT("/Game/Study/UI/Dyeing/WBP_DyeingSlot.WBP_DyeingSlot_C"));

	if (SlotClass && ItemList)
	{
		UUSDyeingSlot* NewSlot = CreateWidget<UUSDyeingSlot>(GetWorld(), SlotClass);
		if (NewSlot)
		{
			ItemList->AddChildToVerticalBox(NewSlot);
			NewSlot->SetData(ItemRaw);
		}
		SlotList.Add(NewSlot);
	}
}
