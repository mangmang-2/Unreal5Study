// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USEnemy.h"
#include "AI/USEnemyAIController.h"
#include "AbilitySystemComponent.h"
#include "ProceduralMeshComponent.h"

AUSEnemy::AUSEnemy()
{
	AIControllerClass = AUSEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ASCComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

void AUSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//int32 NumberOfSections = ceilf(HalfAngle * 2.0f / DeltaAngle);
	//float DAngle = HalfAngle * 2.0f / NumberOfSections;
	//FVector StartPoint = GetActorLocation();
	//StartPoint.Z -= 80;
	//TArray<FHitResult> HitArray;
	//for (int32 i = 0; i < NumberOfSections; i++)
	//{
	//	int32 L_CurrentAngle = DAngle* i - HalfAngle;
	//	
	//	// 회전 축
	//	FVector RotationAxis(0.0f, 0.0f, 1.0f);

	//	// 회전 각도 (도 단위)
	//	float RotationAngle = L_CurrentAngle;

	//	// 벡터 회전
	//	FVector RotatedVector = GetActorForwardVector().RotateAngleAxis(RotationAngle, RotationAxis) * ExternalRadius;
	//	FHitResult HitResult;
	//	HitCheck(StartPoint, StartPoint + RotatedVector, HitResult, false, -1, false);

	//	HitArray.Add(HitResult);
	//}
	//
	//FVector LLeftPoint;
	//FVector LRightPoint;
	//TArray<FVector> Vertices;
	//TArray<int32> Triangles;
	//for (int32 Index = 0; Index < HitArray.Num() - 1; ++Index)
	//{
	//	if (HitArray[Index].bBlockingHit)
	//	{
	//		LLeftPoint = HitArray[Index].Location;
	//	}
	//	else
	//	{
	//		LLeftPoint = HitArray[Index].TraceEnd;
	//	}

	//	if (HitArray[Index + 1].bBlockingHit)
	//	{
	//		LRightPoint = HitArray[Index + 1].Location;
	//	}
	//	else
	//	{
	//		LRightPoint = HitArray[Index + 1].TraceEnd;
	//	}

	//	
	//	Vertices.Add(StartPoint);
	//	Vertices.Add(LLeftPoint);
	//	Vertices.Add(LRightPoint);
	//	
	//	Triangles.Add(0 + Index * 3);
	//	Triangles.Add(2 + Index * 3);
	//	Triangles.Add(1 + Index * 3);
	//}
	//TArray<FVector> Normals;
	//TArray<FVector2D> UVs;
	//TArray<FProcMeshTangent> Tangents;
	//TArray<FColor> VertexColors;
	//ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
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
