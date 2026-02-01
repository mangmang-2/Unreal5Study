// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Object/USObjectGameInstanceSubsystem.h"

void UUSObjectGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadData();
}

void UUSObjectGameInstanceSubsystem::LoadData()
{
    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/ProjectReview/MiniMap/Data/ObjectData.ObjectData"));

    if (DataTable == nullptr)
    {
        return;
    }

    TArray<FMapObjecter*> AllRows;
    DataTable->GetAllRows(nullptr, AllRows);

    for (auto& Row : AllRows)
    {
        Objects.Add(*Row);
    }
}
