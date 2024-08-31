// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetOwningActor(AActor* InOwner);

	void NativeConstruct();
	void NativeDestruct();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;
};
