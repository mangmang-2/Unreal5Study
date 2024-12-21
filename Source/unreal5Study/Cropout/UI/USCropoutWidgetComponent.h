// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "USCropoutWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCropoutWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
