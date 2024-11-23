// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "USVillagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UUSVillagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */


class UNREAL5STUDY_API IUSVillagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Action(class AActor* VillagerAction) = 0;
	virtual void ChangeJob(FName NewJob) = 0;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float PlayDeliverAnim();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayWorkAnim(float Delay);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnToDefaultBT();
};
