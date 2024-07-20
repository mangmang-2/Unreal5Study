// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_IdleState.h"
#include "../../USCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_IdleState::UGameplayAbility_IdleState()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_IdleState::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUSCharacterBase* ABCharacter = CastChecked<AUSCharacterBase>(ActorInfo->AvatarActor.Get());
	if (ABCharacter == nullptr)
		return;
	if (ActionMontage == nullptr)
		return;

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("IdleState"), ActionMontage, 1.0f);
	PlayAttackTask->ReadyForActivation();
}
