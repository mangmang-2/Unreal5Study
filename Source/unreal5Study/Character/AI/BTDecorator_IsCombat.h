// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCombat.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UBTDecorator_IsCombat : public UBTDecorator
{
	GENERATED_BODY()
	

public:
	UBTDecorator_IsCombat();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
