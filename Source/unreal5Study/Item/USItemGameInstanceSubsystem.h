// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "USItemData.h"
#include "USItemGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSItemGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void LoadData();
	void SetOriginColor(FUSItemData& Data);

	void GetItemList(TArray<FUSItemData>& ItemList);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FUSItemData> ItemDataList;
};
