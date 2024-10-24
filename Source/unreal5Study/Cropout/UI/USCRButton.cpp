// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USCRButton.h"
#include "Components/Button.h"

void UUSCRButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UUSCRButton::OnButtonClicked);
	}
}

void UUSCRButton::OnButtonClicked()
{
	OnButtonClick.Broadcast();
}