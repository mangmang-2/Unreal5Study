// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "USCropoutGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCropoutGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent = nullptr;

	UPROPERTY()
	bool bIsMusicPlaying = false;
public:
	void PlayMusic(class USoundBase* Sound, class USoundControlBus* WinLose, class USoundControlBus* Stop , float Volume, bool bPersist);
	void SetGlobalControlBusMixValue(class USoundControlBus* SoundControlBus, float Value, float FadeTime);
	UFUNCTION(BlueprintCallable)
	void NewVoleme(float Value);
};
