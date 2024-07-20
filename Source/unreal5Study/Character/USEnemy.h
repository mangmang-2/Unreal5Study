// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USCharacterBase.h"
#include "Target/USTargetableInterface.h"
#include "Interface/USCharacterAIInterface.h"
#include "USEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSEnemy : public AUSCharacterBase, public IUSTargetableInterface, public IUSCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AUSEnemy();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void PostInitializeComponents() override;

	virtual bool CanBeTargeted() const;
	virtual FVector GetTargetLocation() const;

public:
	virtual float GetAIPatrolRadius();
	virtual float GetAIDetectRange();
	virtual float GetAIAttackRange();
	virtual float GetAITurnSpeed();

	FAICharacterAttackFinished OnAttackFinished;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished);

	virtual void AttackByAI();
	//virtual void NotifyComboActionEnd() override;
	virtual void PossessedBy(AController* NewController) override;
};
