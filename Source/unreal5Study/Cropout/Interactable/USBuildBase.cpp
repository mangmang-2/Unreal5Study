// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USBuildBase.h"
#include "Math/UnrealPlatformMathSSE.h"

void AUSBuildBase::BeginPlay()
{
    Super::BeginPlay();

    SpawnInBuildMode();

    Tags.AddUnique(TEXT("All Resources"));
    ProgressConstruct(0.4);
}

float AUSBuildBase::ProgressConstruct(float InvestedTime)
{
    ProgressionState = (InvestedTime / BuildDifficulty) + ProgressionState;

    int32 LastIndex = MeshList.Num() - 1;
    if (ProgressionState >= LastIndex)
    {
        BuildCompletion();
    }
    else
    {
        UpdateBuildProgress();
    }
    float RemainingConstructionTime = static_cast<float>(LastIndex) - ProgressionState;
    return RemainingConstructionTime;
}

void AUSBuildBase::BuildCompletion()
{
    ConstructComplete();

    int32 LastIndex = MeshList.Num() - 1;

    if (MeshList.IsValidIndex(LastIndex))
    {
        if (Mesh)
		{
			Mesh->SetStaticMesh(MeshList[LastIndex]);
        }
    }

    
}

void AUSBuildBase::ConstructComplete()
{
    Tags.Remove(TEXT("Build"));
}

void AUSBuildBase::UpdateBuildProgress()
{
    int32 CurrentStage = FMath::FloorToInt(ProgressionState);

    if (MeshList.IsValidIndex(CurrentStage))
    {
        if (Mesh)
        {
            Mesh->SetStaticMesh(MeshList[CurrentStage]);
        }
    }
}

float AUSBuildBase::Interact()
{
    Super::Interact();

    return ProgressConstruct(0.4f);
}

void AUSBuildBase::SpawnInBuildMode()
{
    Tags.Add(FName("Build"));

    int32 CurrentStage = FMath::TruncToInt(ProgressionState);

    if (MeshList.IsValidIndex(CurrentStage))
    {
        if (Mesh)
        {
            Mesh->SetStaticMesh(MeshList[CurrentStage]);
        }
    }
}
