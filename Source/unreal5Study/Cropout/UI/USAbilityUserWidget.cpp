// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USAbilityUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UUSAbilityUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	if (IsValid(InOwner))
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InOwner);
	}
}

UAbilitySystemComponent* UUSAbilityUserWidget::GetAbilitySystemComponent() const
{
	return ASC;
}

