// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTTask_Attack.h"

#include "AIController.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Ability/Tag/USGameplayTag.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	FGameplayEventData PayloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ControllingPawn, USTAG_INPUT_ATTACK_COMBOATTAC1, PayloadData);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		})
		, 0.5f, false);

	return EBTNodeResult::InProgress;
}
