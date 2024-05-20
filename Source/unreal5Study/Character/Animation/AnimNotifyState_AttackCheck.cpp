// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotifyState_AttackCheck.h"
#include "../USCharacterBase.h"

void UAnimNotifyState_AttackCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	HitActors.Reset();
}

void UAnimNotifyState_AttackCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		AUSCharacterBase* USCharacterBase = Cast<AUSCharacterBase>(MeshComp->GetOwner());
		if (USCharacterBase)
		{
			USCharacterBase->WeaponAttackCheck(HitActors);
		}
	}
}

void UAnimNotifyState_AttackCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HitActors.Reset();
}
