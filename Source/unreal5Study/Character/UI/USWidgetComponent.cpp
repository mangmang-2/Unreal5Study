// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/USWidgetComponent.h"
#include "Character/UI/USUserWidget.h"

void UUSWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUSUserWidget* GasUserWidget = Cast<UUSUserWidget>(GetWidget());
	if (GasUserWidget)
	{
		GasUserWidget->SetOwningActor(GetOwner());
	}
}
