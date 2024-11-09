// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USBuildConfirm.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSBuildConfirm : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
	FVector2D GetClampedScreenPosition();
	void UpdateBorderPosition(float InDeltaTime);
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> Border;
};
