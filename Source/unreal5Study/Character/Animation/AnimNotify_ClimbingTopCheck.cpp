// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/AnimNotify_ClimbingTopCheck.h"
#include "../USCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimNotify_ClimbingTopCheck::UAnimNotify_ClimbingTopCheck(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsNativeBranchingPoint = true;
}

void UAnimNotify_ClimbingTopCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AUSCharacterBase* USCharacter = Cast<AUSCharacterBase>(MeshComp->GetOwner());
	if (USCharacter)
	{
		USCharacter->ClimbingClear();
	}
}
