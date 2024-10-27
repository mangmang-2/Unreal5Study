// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USBuildItem.h"
#include "../Interactable/USInteractable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "USCostItem.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"

void UUSBuildItem::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUSBuildItem::SetData(FUSResource* Resource)
{
	if (Resource == nullptr)
		return;
	if (Resource->UIIcon == nullptr)
		return;

	TitleImage->SetBrushFromTexture(Resource->UIIcon);
	TitleText->SetText(Resource->Title);
	Border_Color->SetBrushColor(Resource->TabCol);
	UDataTable* DataTablePointer = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Cropout/Spawn/DT_CostTable.DT_CostTable"));

	if (DataTablePointer != nullptr)
	{
		TArray<FUSCostIcon*> AllRows;
		DataTablePointer->GetAllRows(nullptr, AllRows);

		for (const auto& costdata : Resource->Cost)
		{
			for (auto& Row : AllRows)
			{
				if (costdata.Key == Row->CostType)
				{
					TSubclassOf<UUSCostItem> CostItemClass = LoadClass<UUSCostItem>(nullptr, TEXT("/Game/Study/Cropout/UI/WBP_CostItem.WBP_CostItem_C"));
					if (CostItemClass == nullptr)
						return;

					UUSCostItem* CostItem = CreateWidget<UUSCostItem>(GetWorld(), CostItemClass);
					if (CostItem == nullptr)
						return;

					CostItem->SetData(Row->UIIcon, costdata.Value);

					UHorizontalBoxSlot* NewSlot = ResourceList->AddChildToHorizontalBox(CostItem);
					if (NewSlot)
					{
						NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
						NewSlot->SetHorizontalAlignment(HAlign_Center);
						NewSlot->SetVerticalAlignment(VAlign_Center);
					}
					break;
				}
			}
		}
	}
}

