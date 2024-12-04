// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USBaseCrop.h"

void AUSBaseCrop::BeginPlay()
{
    Super::BeginPlay();
    Tags = { FName("Farming"), FName("Ready")};
}


float AUSBaseCrop::Interact()
{
	Super::Interact();

    float Delay;
    int32 Stage;
	FarminProgress(Delay, Stage);
	return Delay;
}

void AUSBaseCrop::FarminProgress(float& Delay, int32& Stage)
{
    if (Tags.Num() > 1)
    {
        Tags.RemoveAt(1);
    }

    // 타이머 설정 및 SwitchStage 호출
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        this,
        &ThisClass::SwitchStage,
        CollectionTime,
        false
    );

    ProgressionState++;
    int32 LastIndex = MeshList.Num() - 1;

    ProgressionState = (ProgressionState > LastIndex) ? 0 : ProgressionState;

    Delay = CollectionTime;
    Stage = ProgressionState;
}

void AUSBaseCrop::SwitchStage()
{
    int32 LastIndex = MeshList.Num() - 1;

    int32 FlooredState = FMath::FloorToInt(ProgressionState);
    if (FlooredState == 0)
    {
        SetReady();
    }
    else
    {
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(
            TimerHandle,
            this,
            &ThisClass::SetReady,
            CooldownTime,
            false
        );
    }
}

void AUSBaseCrop::SetReady()
{
    int32 LastIndex = MeshList.Num() - 1;
    int32 FlooredState = FMath::FloorToInt(ProgressionState);

    bool bIsLastIndex = (FlooredState == LastIndex);

    FName NewTag = bIsLastIndex ? TEXT("Harvest") : TEXT("Ready");

    if (Tags.Num() > 1)
    {
        Tags[1] = NewTag;
    }
    else
    {
        Tags.SetNum(2);
        Tags[1] = NewTag;
    }

    int32 MeshIndex = FMath::TruncToInt(ProgressionState);

    if (MeshList.IsValidIndex(MeshIndex))
    {
        UStaticMesh* NewMesh = MeshList[MeshIndex];

        if (Mesh)
        {
            Mesh->SetStaticMesh(NewMesh);
        }
    }

    PopFarmPlot();
}

void AUSBaseCrop::PopFarmPlot()
{
}

void AUSBaseCrop::SetProgressionsState(float Progression)
{
    Super::SetProgressionsState(Progression);

    SetReady();
}

