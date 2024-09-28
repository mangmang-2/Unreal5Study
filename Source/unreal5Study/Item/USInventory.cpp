// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USInventory.h"
#include "../Data/ModularCharacterDataSubsystem.h"
#include "USItemData.h"

// Sets default values for this component's properties
UUSInventory::UUSInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUSInventory::BeginPlay()
{
	Super::BeginPlay();

	BuildItemList();
}

void UUSInventory::BuildItemList()
{
	// 초기 구성
	if (AActor* Owner = GetOwner())
	{
		// 초기에 가볍게 사용하려고 모듈러를 만들었는데.. 어쩌다보니 공용으로 사용하는데...
		// 아이템 데이터 정의를 새로 해야할 것 같음
		if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(Owner->GetWorld()->GetGameInstance()))
		{
			int32 Row = 0, Column = 0;
			TArray<FModularCharacterRaw> ModularArray;
			ModularSubsystem->GetModularList(ModularArray);
			for (const auto& Modular : ModularArray)
			{
				if (Modular.ModularIcon != nullptr) // 아이콘이 있는것만.. 나중에 수작업이라..
				{
					UUSItemData* item = NewObject<UUSItemData>();
					item->ItemName = Modular.ModularName;
					item->ItemMesh = Modular.ModularMesh;
					item->ItemIcon = Modular.ModularIcon;

					for (int8 i = 0; i < static_cast<int8>(EModularColorParts::MAX); i++)
					{
						FLinearColor LinearColor = ModularSubsystem->GetColor(Modular, i);
						item->ItemOriginPartsColor.Add(i, LinearColor);
						item->ItemChangePartsColor.Add(i, LinearColor); // 초기에는 원본의 색이 들어가 있음
					}

					Itemlist.Add(item);
				}
			}
		}
	}
}

