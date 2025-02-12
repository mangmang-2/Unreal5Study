// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USLayerGame.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSLayerGame : public UUSUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct();

	UFUNCTION()
	void UpdateVillagers(int32 VillagerCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_VillagerCount;
};
