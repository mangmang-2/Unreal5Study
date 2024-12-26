// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cropout/Interactable/USInteractable.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "USBuildBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSBuildBase : public AUSInteractable, public IUSCharacterAIInterface
{
	GENERATED_BODY()

public:
	AUSBuildBase();
	virtual void BeginPlay() override;

	float ProgressConstruct(float InvestedTime);
	void BuildCompletion();
	virtual void ConstructComplete();

	void UpdateBuildProgress();

	virtual float Interact() override;

	void SpawnInBuildMode();


protected:

	float BuildDifficulty = 1.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY()
	TObjectPtr<class UUSCropoutStat> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UUSCropoutWidgetComponent> HPBarWidgetComponent;

	UPROPERTY(EditAnywhere)
	float HPBarHeight = 240.0f;

	UPROPERTY(EditAnywhere)
	float AICollisionRange = 100.0f;

public:

	virtual float GetAIPatrolRadius() { return 0.0f; };
	virtual float GetAIDetectRange() { return 0.0f; };
	virtual float GetAIAttackRange() { return 100.0f; };
	virtual float GetAITurnSpeed() { return 0.0f; };
	virtual float GetAICollisionRange() { return AICollisionRange; };
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) {};
};
