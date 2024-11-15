// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "USCropoutController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSCropoutController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
};
