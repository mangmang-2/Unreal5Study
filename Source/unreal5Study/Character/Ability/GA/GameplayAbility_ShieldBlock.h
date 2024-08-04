// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_ShieldBlock.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UGameplayAbility_ShieldBlock : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_ShieldBlock();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnCompleteCallback();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ActionMontage;
};
