// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USInteractable.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	None UMETA(DisplayName = "None"),
	Food UMETA(DisplayName = "Food"),
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
};

USTRUCT(BlueprintType)
struct FUSCostIcon : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType CostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> UIIcon;
};

USTRUCT(BlueprintType)
struct FUSResource : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class AUSInteractable> TargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> UIIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor TabCol;
};


UCLASS()
class UNREAL5STUDY_API AUSInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
