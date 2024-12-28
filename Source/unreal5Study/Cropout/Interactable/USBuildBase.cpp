// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USBuildBase.h"
#include "Math/UnrealPlatformMathSSE.h"
#include "../UI/USCropoutWidgetComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemComponent.h"
#include "../Stat/USCropoutStat.h"
#include "../../GameMode/USCropoutGameMode.h"
#include "Kismet/GameplayStatics.h"

AUSBuildBase::AUSBuildBase()
{
    ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
    AttributeSet = CreateDefaultSubobject<UUSCropoutStat>(TEXT("AttributeSet"));

    HPBarWidgetComponent = CreateDefaultSubobject<UUSCropoutWidgetComponent>(TEXT("Widget"));
    HPBarWidgetComponent->SetupAttachment(Mesh);
   
}

void AUSBuildBase::BeginPlay()
{
    Super::BeginPlay();

    SpawnInBuildMode();

    Tags.AddUnique(TEXT("All Resources"));
    ProgressConstruct(0.4);

    int32 AbilityCount = 0;
    for (const auto& StartAbility : StartAbilities)
    {
        FGameplayAbilitySpec StartSpec(StartAbility);
        StartSpec.InputID = AbilityCount++;
        ASC->GiveAbility(StartSpec);
    }

    HPBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, HPBarHeight));
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

    AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(UGameplayStatics::GetGameMode(this));
    if (CropoutGameMode)
    {
        CropoutGameMode->UpdateAllInteractables();
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

    return ProgressConstruct(1.0f);
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
