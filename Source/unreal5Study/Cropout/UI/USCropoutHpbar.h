// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "USAbilityUserWidget.h"
#include "USCropoutHpbar.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCropoutHpbar : public UUSAbilityUserWidget//, public IAbilitySystemInterface
{
	GENERATED_BODY()
	

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);

	void UpdateHpBar();
protected:
	
	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1f;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_Hpbar;
};
