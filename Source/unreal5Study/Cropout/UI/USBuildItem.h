// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "../Interactable/USResourceInterface.h"
#include "../../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "USResourceUIItem.h"
#include "USBuildItem.generated.h"


UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cropout_UI_Message);

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSBuildItem : public UUSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetData(struct FUSResource* Resource);
	void BuildCostItem(struct FUSResource* Resource);
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void AddUI();

	void SetParentBuildUI(class UUSBuild* BuildUI);

	virtual void ResponseMessage(FGameplayTag Channel, const FCropoutResourceValueMessageData& Payload);
	void CheckResource(TMap<enum EResourceType, int32> Resources);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TitleImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TitleText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ResourceList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> Border_Color;

	UPROPERTY()
	TSubclassOf<class AUSInteractable> InteractableClass;

	struct FUSResource* ItemResource = nullptr;
	TMap<enum EResourceType, int32> Cost;

	class UUSBuild* ParentBuildUI = nullptr;

};
