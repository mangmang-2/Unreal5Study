// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USResourceInterface.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
};


// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UUSResourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL5STUDY_API IUSResourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveResource(enum EResourceType& Resource, int32& Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddResource(enum EResourceType Resource, int32 Value);

};
