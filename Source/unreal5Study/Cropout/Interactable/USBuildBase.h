// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USInteractable.h"
#include "USBuildBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSBuildBase : public AUSInteractable
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	float ProgressConstruct(float InvestedTime);
	void BuildCompletion();
	void ConstructComplete();

	void UpdateBuildProgress();

	virtual float Interact() override;

	void SpawnInBuildMode();


protected:
	float ProgressionState = 0;
	float BuildDifficulty = 1.0f;
};
