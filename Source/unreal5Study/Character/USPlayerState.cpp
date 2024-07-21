// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Ability/Attribute/USChracterAttributeSet.h"

AUSPlayerState::AUSPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUSChracterAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AUSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
