// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USInteractable.h"
#include "USResourceActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSResourceActor : public AUSInteractable
{
	GENERATED_BODY()

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual float Interact();

	virtual void RemoveResource_Implementation(enum EResourceType& TargetResouce, int32& Value) override;

	void Death();
public:

	UPROPERTY(EditAnywhere)
	float ResouceAmount = 10.f;
	UPROPERTY(EditAnywhere)
	EResourceType ResourceType;
	UPROPERTY(EditAnywhere)
	int32 CollectionValue = 10;
	UPROPERTY(EditAnywhere)
	bool UseRandomMesh = false;
};
