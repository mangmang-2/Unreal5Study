// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTTaskNode_Combat.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Ability/Tag/USGameplayTag.h"
#include "AbilitySystemComponent.h"

UBTTaskNode_Combat::UBTTaskNode_Combat()
{
	NodeName = TEXT("Combat");
}

EBTNodeResult::Type UBTTaskNode_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* OwnerActor = OwnerComp.GetAIOwner()->GetPawn();
	if (OwnerActor)
	{
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, USTAG_INPUT_STATE_COMBAT, PayloadData);
	}

	return EBTNodeResult::Succeeded;
}
