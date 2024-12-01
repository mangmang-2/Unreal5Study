// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Cropout/Interactable/USResourceInterface.h>
#include "NativeGameplayTags.h"
#include "../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USResourceUIItem.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cropout_UI_Message);


USTRUCT(BlueprintType)
struct FCropoutResourceValueMessageData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTag Verb;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Target = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer InstigatorTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer TargetTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer ContextTags;

	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	EResourceType ResourceType;

	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	int32 Value;
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSResourceUIItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void ResponseMessage(FGameplayTag Channel, const FCropoutResourceValueMessageData& Payload);

protected:
	UPROPERTY(EditAnywhere)
	EResourceType ResourceType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Image_Resource;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Resource;

	FGameplayMessageListenerHandle ListenerHandle;
};
