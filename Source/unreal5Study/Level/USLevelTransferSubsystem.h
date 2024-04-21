// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "USLevelTransferSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSLevelTransferSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void SetTransferSequenceFrameTime(int32 TransferSequenceFrameTime, float DelayTime);

private:

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY()
	TObjectPtr<class ULevelSequencePlayer> FadeInOutPlayer;
};
