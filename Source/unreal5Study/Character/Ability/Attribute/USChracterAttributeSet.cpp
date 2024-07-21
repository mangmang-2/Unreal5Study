// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/Attribute/USChracterAttributeSet.h"
#include "../Tag/USGameplayTag.h"
#include "GameplayEffectExtension.h"

UUSChracterAttributeSet::UUSChracterAttributeSet()
	: Health(100.0f)
{
}

void UUSChracterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void UUSChracterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
}

bool UUSChracterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UUSChracterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if ((GetHealth() <= 0.0f))
	{
		Data.Target.AddLooseGameplayTag(USTAG_CHARACTER_DEATH);
		OnOutOfHealth.Broadcast();
	}
}
