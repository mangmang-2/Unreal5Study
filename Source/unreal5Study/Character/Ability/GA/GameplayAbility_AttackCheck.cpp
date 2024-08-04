// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_AttackCheck.h"
#include "Character/Ability/AT/USAT_AttackTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Tag/USGameplayTag.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
		AActor* HitActor = TargetDataHandle.Get(0)->GetHitResult()->GetActor();
		if (HitActor == nullptr)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
		// 방패로 막히면 내 애니메이션 실패
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (ASC && ASC->HasMatchingGameplayTag(USTAG_CHARACTER_STATE_SHIELD_ACTIVE))
		{
			float DotProduct = FVector::DotProduct(HitActor->GetActorForwardVector(), CurrentActorInfo->AvatarActor->GetActorForwardVector());
			//UE_LOG(LogTemp, Log, TEXT(dot : %f"), DotProduct);
			if (DotProduct < -0.7)
			{
				CurrentActorInfo->SkeletalMeshComponent->GetAnimInstance()->StopAllMontages(1.0f);

				FGameplayEventData EventData;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, USTAG_CHARACTER_STATE_SHIELDBLOCK, EventData);
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
				return;
			}
		}

		// 피격 로직
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());			
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
		FGameplayEventData EventData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, USTAG_CHARACTER_INVINSIBLE, EventData);
	}
	//else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	//{
	//	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

	//	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
	//	if (EffectSpecHandle.IsValid())
	//	{
	//		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
	//	}
	//}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
