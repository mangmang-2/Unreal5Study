// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Preview/USPreviewSlotGrid.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USPreviewSlot.h"
#include "Components/ComboBoxString.h"

void UUSPreviewSlotGrid::NativeConstruct()
{
	Super::NativeConstruct();

	if (ComboBoxString_Category)
	{
		ComboBoxString_Category->ClearOptions();

		ComboBoxString_Category->AddOption(StaticEnum<EItemCategory>()->GetDisplayNameTextByIndex(static_cast<int32>(EItemCategory::MAX)).ToString());
		for (uint8 i = 0; i < static_cast<uint8>(EItemCategory::MAX); i++)
		{
			FString DisplayName = StaticEnum<EItemCategory>()->GetDisplayNameTextByIndex(i).ToString();

			ComboBoxString_Category->AddOption(DisplayName);
		}

		// 기본 선택 항목 설정
		ComboBoxString_Category->SetSelectedIndex(0);

		if (!ComboBoxString_Category->OnSelectionChanged.IsAlreadyBound(this, &UUSPreviewSlotGrid::OnCategorySelectionChanged))
			ComboBoxString_Category->OnSelectionChanged.AddDynamic(this, &ThisClass::OnCategorySelectionChanged);
	}

	Build(EItemCategory::MAX);
}

void UUSPreviewSlotGrid::Build(EItemCategory eCategory)
{
	UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance());

	int32 Row = 0, Column = 0;
	TArray<FModularCharacterRaw> ModularArray;
	ModularSubsystem->GetModularList(ModularArray);
	for (const auto& Modular : ModularArray)
	{
		if (Modular.ModularIcon != nullptr)
		{
			
			if (eCategory >= EItemCategory::MAX)
			{
				CreateSlotWidget(Row, Column++, Modular);
				if (Column == 3) // 이걸..계산해서 할까 하다가 일단은 하드코딩
				{
					Column = 0;
					Row++;
				}
			}
			else
			{
				if (Modular.ModularCategory == eCategory)
				{
					CreateSlotWidget(Row, Column++, Modular);
					if (Column == 3)
					{
						Column = 0;
						Row++;
					}
				}
			}
		}
	}
}

void UUSPreviewSlotGrid::CreateSlotWidget(int32 InRow, int32 InColumn, FModularCharacterRaw ModularRaw)
{
	TSubclassOf<UUSPreviewSlot> SlotClass = LoadClass<UUSPreviewSlot>(nullptr, TEXT("/Game/Study/UI/Modular/WBP_Slot.WBP_Slot_C"));

	if (SlotClass && GridPanel)
	{
		UUSPreviewSlot* NewSlot = CreateWidget<UUSPreviewSlot>(GetWorld(), SlotClass);
		if (NewSlot)
		{
			GridPanel->AddChildToUniformGrid(NewSlot, InRow, InColumn);
			NewSlot->SetData(ModularRaw);
		}
	}
}

void UUSPreviewSlotGrid::OnCategorySelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GridPanel->ClearChildren();
	Build(static_cast<EItemCategory>(StaticEnum<EItemCategory>()->GetIndexByName(*SelectedItem)));
}
