// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Object/ObjectData.h"
#include "USObjectGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSObjectGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadData();

private:
    UPROPERTY()
    TArray<FMapObjectDef> Objects;

public:
    const TArray<FMapObjectDef>& GetObjects() const;
};
