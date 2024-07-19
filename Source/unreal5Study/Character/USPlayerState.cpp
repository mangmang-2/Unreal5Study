// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayerState.h"
#include "AbilitySystemComponent.h"

AUSPlayerState::AUSPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* AUSPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
