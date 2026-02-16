#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectData.generated.h"

UENUM()
enum class EObjectType : uint8
{
    NPC,
    Quest,
    Boss,
    Shop,
    Player
};

USTRUCT(BlueprintType)
struct FMapObjectDef : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class AUSMapObjectBase> TargetClass;

    UPROPERTY(EditAnywhere)
    FString Name;

    UPROPERTY(EditAnywhere)
    EObjectType MarkerType;

    UPROPERTY(EditAnywhere)
    bool bShowOnMiniMap = true;

    // 초기 생성 위치
    UPROPERTY(EditAnywhere)
    FVector ObjectPos;
};