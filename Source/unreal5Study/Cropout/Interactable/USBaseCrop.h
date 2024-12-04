// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USResourceActor.h"
#include "USBaseCrop.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSBaseCrop : public AUSResourceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float Interact() override;
	void FarminProgress(float& Delay, int32& Stage);

	UFUNCTION()
	void SwitchStage();

	UFUNCTION()
	void SetReady();

	void PopFarmPlot();

	virtual void SetProgressionsState(float Progression);
protected:
	float CooldownTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCurveFloat> PopPlot;
};