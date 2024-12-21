// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "USCropoutStat.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCropoutStat : public UAttributeSet
{
	GENERATED_BODY()

public:

	UUSCropoutStat();

	ATTRIBUTE_ACCESSORS(UUSCropoutStat, Health);
	ATTRIBUTE_ACCESSORS(UUSCropoutStat, MaxHealth);

	ATTRIBUTE_ACCESSORS(UUSCropoutStat, Damage);

	ATTRIBUTE_ACCESSORS(UUSCropoutStat, MoveSpeed);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed;

public:

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
