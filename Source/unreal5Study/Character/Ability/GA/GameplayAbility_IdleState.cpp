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

	UAbilityTask_PlayMontageAndWait* PlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("IdleState"), ActionMontage, 1.0f);

	PlayTask->OnCompleted.AddDynamic(this, &UGameplayAbility_IdleState::OnCompleteCallback);
	PlayTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_IdleState::OnInterruptedCallback);


	PlayTask->ReadyForActivation();

}


void UGameplayAbility_IdleState::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_IdleState::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}