// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EquipSword.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UAnimNotify_EquipSword : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	uint8 bIsEquip : 1;
};
