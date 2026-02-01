#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectData.generated.h"

USTRUCT(BlueprintType)
struct FMapObjecter : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 대상 Pawn (NPC)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<APawn> TargetPawn = nullptr;

    // 이름 (표시/디버그용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    // 마커 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> MarkerTexture = nullptr;

    // 크기
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Size = 24.f;

public:
    FVector GetWorldLocation() const
    {
        return TargetPawn ? TargetPawn->GetActorLocation() : FVector::ZeroVector;
    }
};