// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "USAT_AttackTrace.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSAT_AttackTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UUSAT_AttackTrace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUSTA_Attack> TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<class AUSTA_Attack> TargetActorClass;

	UPROPERTY()
	TObjectPtr<class AUSTA_Attack> SpawnedTargetActor;

};
