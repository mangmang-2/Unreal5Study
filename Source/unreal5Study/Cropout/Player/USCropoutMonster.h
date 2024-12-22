// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Player/USCropoutPawn.h"
#include "USCropoutMonster.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSCropoutMonster : public AUSCropoutPawn
{
	GENERATED_BODY()
	

public:
	virtual void Action(class AActor* VillagerAction);
};
