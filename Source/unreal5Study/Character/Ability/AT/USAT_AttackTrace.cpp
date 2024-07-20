// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/AT/USAT_AttackTrace.h"
#include "Character/Ability/TA/USTA_Attack.h"
#include "AbilitySystemComponent.h"

UUSAT_AttackTrace* UUSAT_AttackTrace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AUSTA_Attack> TargetActorClass)
{
	UUSAT_AttackTrace* NewTask = NewAbilityTask<UUSAT_AttackTrace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UUSAT_AttackTrace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UUSAT_AttackTrace::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(AbilityEnded);
}

void UUSAT_AttackTrace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AUSTA_Attack>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UUSAT_AttackTrace::OnTargetDataReadyCallback);
	}
}

void UUSAT_AttackTrace::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && SpawnedTargetActor)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UUSAT_AttackTrace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}

	EndTask();
}
