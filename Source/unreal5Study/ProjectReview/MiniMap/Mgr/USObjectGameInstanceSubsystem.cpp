// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Mgr/USObjectGameInstanceSubsystem.h"

void UUSObjectGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadData();
}

void UUSObjectGameInstanceSubsystem::LoadData()
{
    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/ProjectReview/MiniMap/Data/DT_MapObjectData.DT_MapObjectData"));

    if (DataTable == nullptr)
    {
        return;
    }

    TArray<FMapObjectDef*> AllRows;
    DataTable->GetAllRows(nullptr, AllRows);

    for (auto& Row : AllRows)
    {
        Objects.Add(*Row);
    }
}

const TArray<FMapObjectDef>& UUSObjectGameInstanceSubsystem::GetObjects() const
{
    return Objects;
}
