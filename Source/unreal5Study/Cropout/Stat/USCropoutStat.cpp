// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Stat/USCropoutStat.h"
#include "GameplayEffectExtension.h"

UUSCropoutStat::UUSCropoutStat() :
	MaxHealth(100.0f),
	Damage(0.0f),
	MoveSpeed(50.f)
{
	InitHealth(GetMaxHealth());
}

void UUSCropoutStat::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UUSCropoutStat::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{

		}
	}

	return true;
}

void UUSCropoutStat::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	//if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	//{
	//	Data.Target.AddLooseGameplayTag(ABTAG_CHARACTER_ISDEAD);
	//	OnOutOfHealth.Broadcast();
	//}

	//bOutOfHealth = (GetHealth() <= 0.0f);
}