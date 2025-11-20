// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GA/UGameplayAbility_AirAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UUGameplayAbility_AirAttack::UUGameplayAbility_AirAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUGameplayAbility_AirAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character == nullptr)
    {
        Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
        return;
    }

    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (MovementComp)
    {
        MovementComp->SetMovementMode(EMovementMode::MOVE_Falling);
        UE_LOG(LogTemp, Warning, TEXT("IsFalling%d"), MovementComp->IsFalling());
    }

    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UUGameplayAbility_AirAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{	
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (Character == nullptr)
    {
        Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

