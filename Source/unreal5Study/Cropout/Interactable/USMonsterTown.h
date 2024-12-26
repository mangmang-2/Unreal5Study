// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USBuildBase.h"
#include "USMonsterTown.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSMonsterTown : public AUSBuildBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


protected:
	UFUNCTION()
	void RepeatSummon();

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> MonsterActorClass;
};
