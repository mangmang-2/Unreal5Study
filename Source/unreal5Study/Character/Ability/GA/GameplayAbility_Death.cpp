// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/GameplayAbility_Death.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

UGameplayAbility_Death::UGameplayAbility_Death()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActionMontage == nullptr)
		return;

	ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (AvatarCharacter == nullptr || AvatarCharacter->GetMesh() == nullptr)
		return;
	UAnimInstance* AnimInstance = AvatarCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	UCharacterMovementComponent* MoveComp = AvatarCharacter->GetCharacterMovement();
	if (MoveComp == nullptr)
		return;

	AnimInstance->StopAllMontages(0.0f);
	MoveComp->SetMovementMode(EMovementMode::MOVE_None);
	AvatarCharacter->SetActorEnableCollision(false);
	AvatarCharacter->DisableInput(nullptr);

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DeadState"), ActionMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Death::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Death::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

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
}

void UGameplayAbility_Death::OnCompleteCallback()
{
	ACharacter* AvatarCharacter = Cast<ACharacter>(GetActorInfo().AvatarActor.Get());
	if (AvatarCharacter == nullptr || AvatarCharacter->GetMesh() == nullptr)
		return;
	UAnimInstance* AnimInstance = AvatarCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;
	AnimInstance->StopAllMontages(0.0f);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Death::OnInterruptedCallback()
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
