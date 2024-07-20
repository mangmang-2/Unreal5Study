// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_AttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UAnimNotify_AttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

protected:
	UPROPERTY(EditAnywhere, Meta=(Categories=Event))
	FGameplayTag TriggerGameplayTag;
};
