// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_AttackCheck.h"
#include "Character/Ability/AT/USAT_AttackTrace.h"
#include "AbilitySystemBlueprintLibrary.h"

UGameplayAbility_AttackCheck::UGameplayAbility_AttackCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_AttackCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//CurrentLevel = TriggerEventData->EventMagnitude;

	UUSAT_AttackTrace* AttackTraceTask = UUSAT_AttackTrace::CreateTask(this, TargetActorClass);
	AttackTraceTask->OnComplete.AddDynamic(this, &UGameplayAbility_AttackCheck::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UGameplayAbility_AttackCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_AttackCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());

		//const UABCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABCharacterAttributeSet>();
		//FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		//if (EffectSpecHandle.IsValid())
		//{
		//	//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());			
		//	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

		//	FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
		//	CueContextHandle.AddHitResult(HitResult);
		//	FGameplayCueParameters CueParam;
		//	CueParam.EffectContext = CueContextHandle;

		//	TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		//}

		//FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect);
		//if (BuffEffectSpecHandle.IsValid())
		//{
		//	ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		//}
	}
	else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		/*UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}*/
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
