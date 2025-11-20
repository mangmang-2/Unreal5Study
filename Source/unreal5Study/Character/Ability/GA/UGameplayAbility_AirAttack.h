// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_Attack.h"
#include "UGameplayAbility_AirAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUGameplayAbility_AirAttack : public UGameplayAbility_Attack
{
	GENERATED_BODY()
		
public:
	UUGameplayAbility_AirAttack();

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
