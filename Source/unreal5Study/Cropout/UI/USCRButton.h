// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USCRButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickEvent);

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCRButton : public UUSUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> Button;

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable, Category = "Appearance|Event")
	FOnButtonClickEvent OnButtonClick;

	UFUNCTION()
    void OnButtonClicked();
};
