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
#include "USBuildConfirm.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/USCropoutPlayer.h"

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
	
	InteractableClass = Resource->TargetClass.LoadSynchronous();

	BuildCostItem(Resource);
}

void UUSBuildItem::BuildCostItem(FUSResource* Resource)
{
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

					ItemResource = Resource;
					break;
				}
			}
		}
	}
}

FReply UUSBuildItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AUSCropoutPlayer* PlayerCharacter = Cast<AUSCropoutPlayer>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->BeginBuild(InteractableClass);
		}
	}

	AddUI();

	return FReply::Handled();
}

void UUSBuildItem::AddUI()
{
	TSubclassOf<UUSBuildConfirm> BuildConfirmClass = LoadClass<UUSBuildConfirm>(nullptr, TEXT("/Game/Study/Cropout/UI/WBP_Confirm.WBP_Confirm_C"));
	if (BuildConfirmClass == nullptr)
		return;

	UUSBuildConfirm* BuildConfirm = CreateWidget<UUSBuildConfirm>(GetWorld(), BuildConfirmClass);
	if (BuildConfirm == nullptr)
		return;

	BuildConfirm->AddToViewport();
}

