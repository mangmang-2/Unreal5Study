#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MotionData.generated.h"

UENUM(BlueprintType)
enum class EMotionCount : uint8
{
    None = 0,
    MotionMax = 5, // 하나의 슬롯에는 최대 5개의 모션이 있음
    MotionPresetListNum = 10
};

USTRUCT(BlueprintType)
struct FMotionItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MotionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MotionID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UAnimSequence> Animation;
};

USTRUCT(BlueprintType)
struct FMotionSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMotionItem> MotionList;

    FMotionSlot()
    {
        MotionList.SetNum(static_cast<int32>(EMotionCount::MotionMax));
    }
};

USTRUCT(BlueprintType)
struct FMotionPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMotionSlot> SlotList;

    void Init()
    {
        SlotList.SetNum(static_cast<int32>(EMotionCount::MotionPresetListNum));
    }
};
