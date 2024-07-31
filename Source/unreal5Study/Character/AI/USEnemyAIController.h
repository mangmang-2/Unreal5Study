// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "USEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUSEnemyAIController();
	
	void RunAI();
	void StopAI();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> BTAsset;

protected:
	UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	

public:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class AActor>> PerceptionActors;

	float GetRadius();
	float GetAngleDegrees();
};
