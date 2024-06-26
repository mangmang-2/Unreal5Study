// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_EquipShield.h"
#include "../USCharacterBase.h"

void UAnimNotify_EquipShield::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AUSCharacterBase* USCharacterBase = Cast<AUSCharacterBase>(MeshComp->GetOwner());
		if (USCharacterBase)
		{
			USCharacterBase->ShowShield(bIsEquip);
		}
	}
}
