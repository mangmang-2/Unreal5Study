// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MotionSequence/MotionSeqenceSubsystem.h"

void UMotionSeqenceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadData();
}

void UMotionSeqenceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMotionSeqenceSubsystem::LoadData()
{
    UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/ProjectReview/MotionSeqence/data/DT_MotionData.DT_MotionData"));

    if (DataTable == nullptr)
    {
        return;
    }

    TArray<FMotionItem*> AllRows;
    DataTable->GetAllRows(nullptr, AllRows);

    for (auto& Row : AllRows)
    {
        MotionList.Add(*Row);
    }
}

const TArray<FMotionItem>& UMotionSeqenceSubsystem::GetMotionList()
{
    return MotionList;
}

TArray<FMotionItem> UMotionSeqenceSubsystem::MatchingMotionName(const FString& MotionName)
{
    TArray<FMotionItem> Result;
    for (const auto& Item : MotionList)
    {
        if (Item.MotionName.Contains(MotionName, ESearchCase::IgnoreCase))
        {
            Result.Add(Item);
        }
    }

    return Result;
}

const FMotionItem& UMotionSeqenceSubsystem::GetMotionByID(int32 ID)
{
    for (const auto& Item : MotionList)
    {
        if (Item.MotionID == ID)
        {
            return Item;
        }
    }

    static FMotionItem Empty;
    return Empty;
}
