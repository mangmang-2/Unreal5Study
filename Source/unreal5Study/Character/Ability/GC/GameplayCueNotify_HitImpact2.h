// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayCueNotify_HitImpact2.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UGameplayCueNotify_HitImpact2 : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGameplayCueNotify_HitImpact2();

	virtual void HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	void PlayMontage(class AActor* Target);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ActionMontage;
};
