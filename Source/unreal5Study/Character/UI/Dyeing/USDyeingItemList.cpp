// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingItemList.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USDyeingSlot.h"
#include "Components/VerticalBox.h"

void UUSDyeingItemList::NativeConstruct()
{
	Super::NativeDestruct();


	UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance());

	int32 Row = 0, Column = 0;
	TArray<FModularCharacterRaw> ModularArray;
	ModularSubsystem->GetModularList(ModularArray);
	for (const auto& Modular : ModularArray)
	{
		if (Modular.ModularIcon != nullptr)
		{
			CreateSlotWidget(Modular);
		}
	}
}

void UUSDyeingItemList::CreateSlotWidget(FModularCharacterRaw ModularRaw)
{
	TSubclassOf<UUSDyeingSlot> SlotClass = LoadClass<UUSDyeingSlot>(nullptr, TEXT("/Game/Study/UI/Dyeing/WBP_DyeingSlot.WBP_DyeingSlot_C"));

	if (SlotClass && ItemList)
	{
		UUSDyeingSlot* NewSlot = CreateWidget<UUSDyeingSlot>(GetWorld(), SlotClass);
		if (NewSlot)
		{
			ItemList->AddChildToVerticalBox(NewSlot);
			NewSlot->SetData(ModularRaw);
		}
	}
}
