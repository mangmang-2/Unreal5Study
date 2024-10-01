// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USItemGameInstanceSubsystem.h"

void UUSItemGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadData();
}

void UUSItemGameInstanceSubsystem::LoadData()
{
    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Data/ModularCharacter/ItemData.ItemData"));

    if (DataTable == nullptr)
    {
        return;
    }

    TArray<FUSItemData*> AllRows;
    DataTable->GetAllRows(nullptr, AllRows);

    for (auto& Row : AllRows)
    {
		SetOriginColor(*Row);

        ItemDataList.Add(*Row);
    }
}

void UUSItemGameInstanceSubsystem::SetOriginColor(FUSItemData& Data)
{
	USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
	SkeletalMeshComponent->SetSkeletalMesh(Data.ItemMesh);

	FLinearColor Color = FLinearColor::Black;
	if (SkeletalMeshComponent)
	{
		UMaterialInstanceDynamic* MaterialInstance = SkeletalMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
		if (MaterialInstance)
		{
			for (uint8 ColorParts = 0 ; ColorParts < static_cast<uint8>(EItemColorParts::MAX); ColorParts++ )
			{
				FString ColorPartsName = StaticEnum<EItemColorParts>()->GetDisplayNameTextByIndex(ColorParts).ToString();
				MaterialInstance->GetVectorParameterValue(FName(*ColorPartsName), Color);
				Data.ItemOriginPartsColor.Add(ColorParts, Color);
				Data.ItemChangePartsColor.Add(ColorParts, Color);
			}
		}
	}
}

void UUSItemGameInstanceSubsystem::GetItemList(TArray<FUSItemData>& ItemList)
{
	ItemList = ItemDataList;
}
