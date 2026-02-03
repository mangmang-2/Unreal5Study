// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "USObjectWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSObjectWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
    void SpawnMapObjects();

    bool IsAlreadySpawned(const FVector& Pos) const;

private:
    UPROPERTY()
    TArray<TObjectPtr<AActor>> SpawnedActors;
};
