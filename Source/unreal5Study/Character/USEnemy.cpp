// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USEnemy.h"
#include "AI/USEnemyAIController.h"
#include "AbilitySystemComponent.h"

AUSEnemy::AUSEnemy()
{
	AIControllerClass = AUSEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ASCComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

void AUSEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

bool AUSEnemy::CanBeTargeted() const
{
	return true;
}

FVector AUSEnemy::GetTargetLocation() const
{
	return GetActorLocation();
}

float AUSEnemy::GetAIPatrolRadius()
{
	return 1000.0f;
}

float AUSEnemy::GetAIDetectRange()
{
	return 800.0f;
}

float AUSEnemy::GetAIAttackRange()
{
	return 200;
}

float AUSEnemy::GetAITurnSpeed()
{
	return 2.0f;
}

void AUSEnemy::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AUSEnemy::AttackByAI()
{
	/*if (CurrentCombo == 0)
	{
		CurrentCombo++;
		const float AttackSpeedRate = 1.0f;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(NormalAttackMontage, AttackSpeedRate);

		float ComboEffectiveTime = NormalAttackMontage->GetSectionLength(CurrentCombo - 1) * 0.8;

		if (ComboEffectiveTime > 0.0f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
				OnAttackFinished.ExecuteIfBound();
				})
				, ComboEffectiveTime, false);
		}

	}
	else if (NormalAttackMontage->GetNumSections() == CurrentCombo)
	{
		ComboActionEnd();
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, NormalAttackMontage->GetNumSections());
		AnimInstance->Montage_JumpToSection(NormalAttackMontage->GetSectionName(CurrentCombo - 1), NormalAttackMontage);
		float ComboEffectiveTime = NormalAttackMontage->GetSectionLength(CurrentCombo - 1) * 0.8;

		if (ComboEffectiveTime > 0.0f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
				OnAttackFinished.ExecuteIfBound();
				})
				, ComboEffectiveTime, false);
		}
	}*/
}

void AUSEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASCComponent->InitAbilityActorInfo(this, this);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASCComponent->GiveAbility(StartSpec);
	}
}

//void AUSEnemy::NotifyComboActionEnd()
//{
//	Super::NotifyComboActionEnd();
//	OnAttackFinished.ExecuteIfBound();
//}
