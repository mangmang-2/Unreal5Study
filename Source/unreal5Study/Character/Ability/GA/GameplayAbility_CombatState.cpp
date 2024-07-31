// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_CombatState.h"
#include "../../USCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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

	UAbilityTask_PlayMontageAndWait* PlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("CombatState"), ActionMontage, 1.0f);

	PlayTask->OnCompleted.AddDynamic(this, &UGameplayAbility_CombatState::OnCompleteCallback);
	PlayTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_CombatState::OnInterruptedCallback);

	PlayTask->ReadyForActivation();
}

void UGameplayAbility_CombatState::OnCompleteCallback()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (ASC)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		//EffectContext.AddSourceObject(ActorInfo->AvatarActor.Get());
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_CombatState::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
