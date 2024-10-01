// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Item/USItemData.h"
#include "ModularCharacterDataSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FModularCharacterRaw : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ModularName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ModularCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<class USkeletalMesh> ModularMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<class UTexture2D> ModularIcon;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPreviewChange, struct FModularCharacterRaw, ModularRaw);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreviewClear);

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

	FLinearColor GetColor(FModularCharacterRaw Modular, uint8 ColorParts);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FModularCharacterRaw> ModularCharacterDataArray;

public:
	void GetModularList(TArray<FModularCharacterRaw>& ModularArray);
	void GetModularList(EItemCategory eCategory, TArray<FModularCharacterRaw>& ModularArray);
	bool GetRandomModular(EItemCategory eCategory, FModularCharacterRaw& ModularData);

public:
	FOnPreviewChange OnPreviewChange;
	FOnPreviewChange OnPreviewMainChange;
	FOnPreviewClear OnPreviewClear;
};
