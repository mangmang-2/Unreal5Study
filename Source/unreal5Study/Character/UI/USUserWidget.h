// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USWidgetID.h"
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

	UFUNCTION()
	void DelegateMessage(int32 MessageType, class UWidgetMessage* WidgetMessage);

	virtual void ResponseMessage(int32 MessageType, class UWidgetMessage* WidgetMessage);
	virtual void SendMessage(EWidgetID SendWidgetID, int32 MessageType, class UWidgetMessage* WidgetMessage);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwningActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "WidgetID")
	EWidgetID WidgetID = EWidgetID::None;
	 
};
