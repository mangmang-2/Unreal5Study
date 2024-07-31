// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTDecorator_IsCombat.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Ability/Tag/USGameplayTag.h"
#include "AbilitySystemComponent.h"

UBTDecorator_IsCombat::UBTDecorator_IsCombat()
{
	NodeName = TEXT("IsCombat");
}

bool UBTDecorator_IsCombat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerComp.GetAIOwner()->GetPawn());
	if (ASC == nullptr)
		return false;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(USTAG_CHARACTER_STATE_COMBAT);
	return ASC->HasAllMatchingGameplayTags(TagContainer);
}
