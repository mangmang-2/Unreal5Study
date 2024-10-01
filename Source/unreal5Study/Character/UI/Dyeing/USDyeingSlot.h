// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "Item/USItemData.h"
#include "USDyeingData.h"
#include "Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USDyeingSlot.generated.h"


/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingSlot : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();
	void ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload);

	void SetData(FUSItemData _ItemData);

	const FUSItemData GetItemData();

	UFUNCTION(BlueprintCallable)
	void OnModularPartsIconButtonClicked();

	void SetColorImage();

protected:
	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ItemName;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_0;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_4;

	UPROPERTY()
	TArray<TObjectPtr<class UImage>> ImageArray;

	FUSItemData ItemData;
};
