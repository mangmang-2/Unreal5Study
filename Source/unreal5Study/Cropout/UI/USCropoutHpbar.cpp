// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USCropoutHpbar.h"
#include "../Stat/USCropoutStat.h"
#include "Components/ProgressBar.h"


void UUSCropoutHpbar::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UUSCropoutStat::GetHealthAttribute()).AddUObject(this, &UUSCropoutHpbar::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UUSCropoutStat::GetMaxHealthAttribute()).AddUObject(this, &UUSCropoutHpbar::OnMaxHealthChanged);

		const UUSCropoutStat* CurrentAttributeSet = ASC->GetSet<UUSCropoutStat>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();

			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UUSCropoutHpbar::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
}

void UUSCropoutHpbar::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
}

void UUSCropoutHpbar::UpdateHpBar()
{
	if (ProgressBar_Hpbar)
	{
		ProgressBar_Hpbar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

}
