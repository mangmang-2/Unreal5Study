// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USCropoutWidgetComponent.h"
#include "USAbilityUserWidget.h"

void UUSCropoutWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUSAbilityUserWidget* AbilityWidget = Cast<UUSAbilityUserWidget>(GetWidget());
	if (AbilityWidget)
	{
		AbilityWidget->SetAbilitySystemComponent(GetOwner());
	}
}
