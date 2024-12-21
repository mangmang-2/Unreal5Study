// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USAbilityUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSAbilityUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetAbilitySystemComponent(AActor* InOwner);
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;
	
};
