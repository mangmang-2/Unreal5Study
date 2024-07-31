// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Combat.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UBTTaskNode_Combat : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_Combat();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
