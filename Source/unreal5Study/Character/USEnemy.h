// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USCharacterBase.h"
#include "Target/USTargetableInterface.h"
#include "USEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSEnemy : public AUSCharacterBase, public IUSTargetableInterface
{
	GENERATED_BODY()
	
public:
	AUSEnemy();

	virtual bool CanBeTargeted() const;
	virtual FVector GetTargetLocation() const;
};
