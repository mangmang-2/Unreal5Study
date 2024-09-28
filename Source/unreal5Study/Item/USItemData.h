// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "USItemData.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSItemData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USkeletalMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> ItemIcon;

	TMap<int8, FLinearColor> ItemOriginPartsColor;
	TMap<int8, FLinearColor> ItemChangePartsColor;
};
