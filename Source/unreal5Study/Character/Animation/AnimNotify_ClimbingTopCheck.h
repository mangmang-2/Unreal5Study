// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ClimbingTopCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UAnimNotify_ClimbingTopCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_ClimbingTopCheck(const FObjectInitializer& ObjectInitializer);

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
	
};
