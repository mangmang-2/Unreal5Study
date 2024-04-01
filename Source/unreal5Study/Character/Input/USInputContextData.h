// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USInputContextData.generated.h"



UENUM(BlueprintType)
enum class EInputContextType : uint8
{
	None UMETA(DisplayName = "None"),
	FirstPerson UMETA(DisplayName = "FirstPerson"),
	ThirdPerson UMETA(DisplayName = "ThirdPerson"),
	TopDown UMETA(DisplayName = "TopDown"),
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSInputContextData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputMappingContext> InputMappingContext;
};
