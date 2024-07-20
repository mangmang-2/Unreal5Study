// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "USTA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSTA_Attack : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	AUSTA_Attack();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;
};
