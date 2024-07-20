// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_CombatState.h"
#include "../../USCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGameplayAbility_CombatState::UGameplayAbility_CombatState()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_CombatState::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AUSCharacterBase* ABCharacter = CastChecked<AUSCharacterBase>(ActorInfo->AvatarActor.Get());
	if (ABCharacter == nullptr)
		return;
	if (ActionMontage == nullptr)
		return;

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("CombatState"), ActionMontage, 1.0f);
	PlayAttackTask->ReadyForActivation();
}
