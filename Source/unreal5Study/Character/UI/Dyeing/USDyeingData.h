#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USDyeingData.generated.h"

USTRUCT(BlueprintType)
struct FDyeingMessageData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTag Verb;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Target = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer InstigatorTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer TargetTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer ContextTags;

	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	double Magnitude = 1.0;

	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	uint8 MessageType = 0;

	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	uint8 PartsType = 0;
	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	uint8 ColorParts = 0;
	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	FLinearColor Color;
	UPROPERTY(BlueprintReadWrite, Category=Gameplay)
	FModularCharacterRaw ModularData;
};
