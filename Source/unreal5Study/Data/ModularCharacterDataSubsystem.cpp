// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ModularCharacterDataSubsystem.h"

UModularCharacterDataSubsystem::UModularCharacterDataSubsystem()
{
}

void UModularCharacterDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    ReadDataTable();
}

void UModularCharacterDataSubsystem::ReadDataTable()
{
    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Data/ModularCharacter/ModularCharacterTable.ModularCharacterTable"));

    if (DataTable == nullptr)
    {
        return;
    }

    TArray<FModularCharacterRaw*> AllRows;
    DataTable->GetAllRows(nullptr, AllRows);

    for (auto& Row : AllRows)
    {
        ModularCharacterDataArray.Add(*Row);
    }
}

FLinearColor UModularCharacterDataSubsystem::GetColor(FModularCharacterRaw Modular, uint8 ColorParts)
{
    USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
    SkeletalMeshComponent->SetSkeletalMesh(Modular.ModularMesh);
    FLinearColor Color = FLinearColor::Black;
    if (SkeletalMeshComponent)
    {
        UMaterialInstanceDynamic* MaterialInstance = SkeletalMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
        if (MaterialInstance)
        {
            FString ColorPartsName = StaticEnum<EModularColorParts>()->GetDisplayNameTextByIndex(ColorParts).ToString();
            bool bHasColor = MaterialInstance->GetVectorParameterValue(FName(*ColorPartsName), Color);
        }
    }
    return Color;
}

void UModularCharacterDataSubsystem::GetModularList(TArray<FModularCharacterRaw>& ModularArray)
{
    ModularArray = ModularCharacterDataArray;
}

void UModularCharacterDataSubsystem::GetModularList(EModularCharacterType eCategory, TArray<FModularCharacterRaw>& ModularArray)
{
    for (auto& Row : ModularCharacterDataArray)
    {
        if (Row.ModularCategory == eCategory)
        {
            ModularArray.Add(Row);
        }
    }
}

bool UModularCharacterDataSubsystem::GetRandomModular(EModularCharacterType eCategory, FModularCharacterRaw& ModularData)
{
    TArray<FModularCharacterRaw> ModularArray;
    GetModularList(eCategory, ModularArray);

    if (ModularArray.IsEmpty())
        return false; 

    int32 RandomIndex = FMath::RandRange(0, ModularArray.Num() - 1);

    // 랜덤하게 선택된 요소를 반환합니다.
    ModularData = ModularArray[RandomIndex];

    return true;
}
