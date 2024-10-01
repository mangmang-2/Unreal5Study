// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "USItemData.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Helmet			UMETA(DisplayName = "Helmet"),
	Helmet2			UMETA(DisplayName = "Helmet2"),
	Gloves			UMETA(DisplayName = "Gloves"),
	Pants			UMETA(DisplayName = "Pants"),
	ArmArmor		UMETA(DisplayName = "ArmArmor"),
	PantsArmor		UMETA(DisplayName = "PantsArmor"),
	Shoes			UMETA(DisplayName = "Shoes"),
	ShoulderArmor	UMETA(DisplayName = "ShoulderArmor"),
	Torso			UMETA(DisplayName = "Torso"),
	TorsoRag		UMETA(DisplayName = "TorsoRag"), 
	TorsoArms		UMETA(DisplayName = "TorsoArms"),
	HelmetClose		UMETA(DisplayName = "HelmetClose"),
	Coat			UMETA(DisplayName = "Coat"),
	MAX				UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EItemColorParts : uint8
{
	Color			UMETA(DisplayName = "2 color"),
	Base			UMETA(DisplayName = "base"),
	Belts			UMETA(DisplayName = "belts"),
	Damage			UMETA(DisplayName = "damage"),
	Metal			UMETA(DisplayName = "metal"),
	MAX				UMETA(DisplayName = "None"),
};


/**
 * 
 */
USTRUCT(BlueprintType)
struct FUSItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> ItemIcon;

	TMap<int8, FLinearColor> ItemOriginPartsColor;
	TMap<int8, FLinearColor> ItemChangePartsColor;

public:

	FLinearColor GetOriginColor(uint8 ColorParts) const;
	FLinearColor GetChangeColor(uint8 ColorParts) const;

	void SetOriginColor(uint8 ColorParts, FLinearColor Color);
	void SetChangeColor(uint8 ColorParts, FLinearColor Color);
};
