// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/GameInstance/USCropoutGameInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AudioDevice.h"
#include "AudioModulationStatics.h"
#include "SoundControlBus.h"

void UUSCropoutGameInstance::PlayMusic(class USoundBase* Sound, class USoundControlBus* WinLose, class USoundControlBus* Stop, float Volume, bool bPersist)
{
    if (!Sound) return;

    if (bIsMusicPlaying)
    {
        SetGlobalControlBusMixValue(WinLose, 0.5f, 0.0f);
        SetGlobalControlBusMixValue(Stop, 0.0f, 0.0f);
    }
    else
    {
        AudioComponent = UGameplayStatics::CreateSound2D(this, Sound, Volume, 1.0f, 0.0f, nullptr, bPersist);
        if (AudioComponent)
        {
            AudioComponent->bAutoDestroy = false;

            AudioComponent->Play();
            bIsMusicPlaying = true;
        }
    }
}

void UUSCropoutGameInstance::SetGlobalControlBusMixValue(class USoundControlBus* SoundControlBus, float Value, float FadeTime)
{
    UAudioModulationStatics::SetGlobalBusMixValue(GetWorld(), SoundControlBus, Value, FadeTime);
}

void UUSCropoutGameInstance::NewVoleme(float Value)
{
    if (AudioComponent)
    {
        AudioComponent->SetVolumeMultiplier(Value);
    }
}
