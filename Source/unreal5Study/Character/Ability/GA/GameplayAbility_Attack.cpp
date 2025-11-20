// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Attack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UGameplayAbility_Attack::UGameplayAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActionMontage == nullptr)
		return;
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ActionMontage, 1.0f, GetNextSection());
	PlayAttackTask->OnCompleted.AddDynamic(this, &UGameplayAbility_Attack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_Attack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	StartComboTimer();
}

void UGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboInput = false;
	}
	else
	{
		HasNextComboInput = true;
		SetFalling(Handle, ActorInfo, ActivationInfo);
	}
}


void UGameplayAbility_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	SetFalling(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentCombo = 0;
	HasNextComboInput = false;
	SetFalling(Handle, ActorInfo, ActivationInfo);
}

void UGameplayAbility_Attack::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Attack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

FName UGameplayAbility_Attack::GetNextSection()
{
	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ActionMontage->GetNumSections());
	return ActionMontage->GetSectionName(CurrentCombo - 1);
}

void UGameplayAbility_Attack::StartComboTimer()
{
	float ComboEffectiveTime = ActionMontage->GetSectionLength(CurrentCombo - 1) * 0.5;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UGameplayAbility_Attack::CheckComboInput, ComboEffectiveTime, false);
	}
}

void UGameplayAbility_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboInput)
	{
		HasNextComboInput = false;
		MontageJumpToSection(GetNextSection());
		StartComboTimer();
		if (CurrentCombo >= ActionMontage->GetNumSections())
			CurrentCombo = 0;
	}
}

void UGameplayAbility_Attack::SetFalling(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->SetMovementMode(EMovementMode::MOVE_Falling);
		UE_LOG(LogTemp, Warning, TEXT("CancelAbility IsFalling%d"), MovementComp->IsFalling());
	}
}
