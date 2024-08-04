// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_ShieldBlock.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_ShieldBlock::UGameplayAbility_ShieldBlock()
{
}

void UGameplayAbility_ShieldBlock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActionMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ShieldBlock"), ActionMontage, 1.0f);
		PlayAttackTask->OnCompleted.AddDynamic(this, &UGameplayAbility_ShieldBlock::OnCompleteCallback);
		PlayAttackTask->ReadyForActivation();
	}
}

void UGameplayAbility_ShieldBlock::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


