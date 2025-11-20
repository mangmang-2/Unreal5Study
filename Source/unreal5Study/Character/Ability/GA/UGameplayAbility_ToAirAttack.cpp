// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/UGameplayAbility_ToAirAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UUGameplayAbility_ToAirAttack::UUGameplayAbility_ToAirAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUGameplayAbility_ToAirAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (AirAttackMontage == nullptr)
        return;

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character == nullptr)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->SetMovementMode(EMovementMode::MOVE_Flying);
    }

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ToAirAttack"), AirAttackMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UUGameplayAbility_ToAirAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UUGameplayAbility_ToAirAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void UUGameplayAbility_ToAirAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UUGameplayAbility_ToAirAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
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

    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UUGameplayAbility_ToAirAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
        UE_LOG(LogTemp, Warning, TEXT("IsFalling%d"), MovementComp->IsFalling());
    }
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UUGameplayAbility_ToAirAttack::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UUGameplayAbility_ToAirAttack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
