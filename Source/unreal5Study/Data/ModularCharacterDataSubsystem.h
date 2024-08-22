// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ModularCharacterDataSubsystem.generated.h"


UENUM(BlueprintType)
enum class EModularCharacterType : uint8
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
	MAX				UMETA(DisplayName = "Max"),
};

USTRUCT(BlueprintType)
struct FModularCharacterRaw : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ModularName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModularCharacterType ModularCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<class USkeletalMesh> ModularMesh;
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UModularCharacterDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UModularCharacterDataSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void ReadDataTable();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FModularCharacterRaw> ModularCharacterDataArray;

public:
	void GetModularList(EModularCharacterType eCategory, TArray<FModularCharacterRaw>& ModularArray);
	bool GetRandomModular(EModularCharacterType eCategory, FModularCharacterRaw& ModularData);

	
};
