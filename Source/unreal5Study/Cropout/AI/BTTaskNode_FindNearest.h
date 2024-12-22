// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_FindNearest.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UBTTaskNode_FindNearest : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    FBlackboardKeySelector TargetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
    FBlackboardKeySelector Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
    FBlackboardKeySelector NearestTo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
    TArray<AActor*> PossibleActors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
    bool bPathFound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tag")
    AActor* NewTarget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
    TSubclassOf<AActor> TargetTownClass;
};
