// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/GC/GameplayCueNotify_HitImpact2.h"
#include "GameFramework/Character.h"

UGameplayCueNotify_HitImpact2::UGameplayCueNotify_HitImpact2()
{
}

void UGameplayCueNotify_HitImpact2::HandleGameplayCue(AActor* Target, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	PlayMontage(Target);
}

void UGameplayCueNotify_HitImpact2::PlayMontage(class AActor* Target)
{
	if (Target == nullptr)
		return;
	ACharacter* character = Cast<ACharacter>(Target);
	if (character && character->GetMesh())
	{
		UAnimInstance* AnimInstance = character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(ActionMontage);
		}
	}
}
