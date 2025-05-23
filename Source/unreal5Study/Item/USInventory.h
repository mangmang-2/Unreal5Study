// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "Character/UI/Dyeing/USDyeingData.h"
#include "USInventory.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL5STUDY_API UUSInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUSInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void BuildItemList();

	void ResponseMessage(FGameplayTag Channel, const FDyeingMessageData& Payload);

protected:
	
	UPROPERTY()
	TArray<struct FUSItemData> Itemlist;
};
