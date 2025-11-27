// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MotionData.h"
#include "MotionSeqenceSubsystem.generated.h"

/**
  
  [Data Table]  ¡æ  [UMotionSequenceSubsystem]  ¡æ  [MotionMainWidget]
                                   ¡é
                             [PlayerCharacter]
                                   ¡é
                         [Animation Play Logic]
 */
UCLASS()
class UNREAL5STUDY_API UMotionSeqenceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void LoadData();
    const TArray<FMotionItem>& GetMotionList();
    TArray<FMotionItem> MatchingMotionName(const FString& MotionName);
    const FMotionItem& GetMotionByID(int32 ID);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMotionItem> MotionList;

};
