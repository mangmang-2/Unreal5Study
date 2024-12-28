// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Cropout/Interactable/USResourceInterface.h"
#include "USCropoutSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSTSaveInteract
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTransform Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AUSInteractable> Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Tag;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Progress;
};

USTRUCT(BlueprintType)
struct FSTVillager
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Task;
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCropoutSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite )
    TMap<EResourceType, int32> Resources;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite )
    TArray<FSTSaveInteract> Interactables;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSTVillager> Villagers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SeedValue;
};
