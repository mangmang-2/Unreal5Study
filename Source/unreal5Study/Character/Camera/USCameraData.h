// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "USCameraData.generated.h"

UENUM(BlueprintType)
enum class EViewType : uint8
{
	None UMETA(DisplayName = "None"),
	FirstPerson UMETA(DisplayName = "FirstPerson"),
	ThirdPerson UMETA(DisplayName = "ThirdPerson"),
	TopDown UMETA(DisplayName = "TopDown"),
	Max,
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSCameraData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bUseControllerRotationYaw;

};
