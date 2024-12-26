// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USMonsterTown.h"
#include "NavigationSystem.h"
#include "../Player/USCropoutMonster.h"
#include "../../GameMode/USCropoutGameMode.h"

void AUSMonsterTown::BeginPlay()
{
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        TimerHandle,
        this,
        &AUSMonsterTown::RepeatSummon,
        1.0f, //10.0f,
        false //true
    );
}

void AUSMonsterTown::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AUSMonsterTown::RepeatSummon()
{
    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
    if (GameMode)
    {
        AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(GameMode);
        if (CropoutGameMode)
        {
            CropoutGameMode->SpawnMonster(MonsterActorClass);
        }
    }
}
