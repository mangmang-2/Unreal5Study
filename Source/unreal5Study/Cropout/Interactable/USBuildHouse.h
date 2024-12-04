// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USBuildBase.h"
#include "USBuildHouse.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSBuildHouse : public AUSBuildBase
{
	GENERATED_BODY()

public:
	void SpawnVillagers();
	virtual void ConstructComplete();
};
