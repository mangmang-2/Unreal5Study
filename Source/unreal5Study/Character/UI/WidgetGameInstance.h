// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WidgetGameInstance.generated.h"


UCLASS()
class UNREAL5STUDY_API UWidgetMessage : public UObject
{
    GENERATED_BODY()
public:
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetMessage, int32, MessageType, UWidgetMessage*, WidgetMessage);
/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UWidgetGameInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	TMap<uint32, FOnWidgetMessage> WidgetMessageMap;
};
