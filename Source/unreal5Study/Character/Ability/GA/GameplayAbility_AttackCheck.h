// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UGameplayAbility_AttackCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_AttackCheck();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class AUSTA_Attack> TargetActorClass;
};
