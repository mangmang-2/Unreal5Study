// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTService_Detect.h"
#include "BTService_Detect.h"
#include "AIController.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "USEnemyAIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../Ability/Tag/USGameplayTag.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	IUSCharacterAIInterface* AIPawn = Cast<IUSCharacterAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	AUSEnemyAIController* AIController = Cast<AUSEnemyAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return;

	DrawSightCone(ControllingPawn, AIController);
	float DetectRadius = AIPawn->GetAIDetectRange();

	if (AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target"))) ;  Actor != nullptr)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		if (ASC == nullptr)
			return;

		if (ASC->HasMatchingGameplayTag(USTAG_CHARACTER_STATE_DEATH))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
			return;
		}
		

		float Length = (Center - Actor->GetActorLocation()).Length();
		if (Length < DetectRadius)
			return;
	}

	for (auto& Actor : AIController->PerceptionActors)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		if (ASC == nullptr)
			continue;

		if (ASC->HasMatchingGameplayTag(USTAG_CHARACTER_STATE_DEATH))
			continue;

		float Length = (Center - Actor->GetActorLocation()).Length();
		if (Length < DetectRadius)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);

			DrawDebugLine(World, ControllingPawn->GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);

			return;
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);;
}

void UBTService_Detect::DrawSightCone(APawn* ControllingPawn, AUSEnemyAIController* AIController)
{
	if (ControllingPawn)
	{
		float Radius = AIController->GetRadius();
		float HalfAngle = AIController->GetAngleDegrees();

		int32 NumberOfSections = ceilf(HalfAngle * 2.0f / 1.0f);
		float DAngle = HalfAngle * 2.0f / NumberOfSections;
		FVector StartPoint = ControllingPawn->GetActorLocation();
		StartPoint.Z -= 90;
		TArray<FHitResult> HitArray;
		for (int32 i = 0; i < NumberOfSections; i++)
		{
			int32 L_CurrentAngle = DAngle * i - HalfAngle;

			// 회전 축
			FVector RotationAxis(0.0f, 0.0f, 1.0f);

			// 회전 각도 (도 단위)
			float RotationAngle = L_CurrentAngle;

			// 벡터 회전
			FVector RotatedVector = ControllingPawn->GetActorForwardVector().RotateAngleAxis(RotationAngle, RotationAxis) * Radius;
			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(ControllingPawn); // 이 액터는 트레이스에서 제외
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartPoint,
				StartPoint + RotatedVector,
				ECC_Pawn,
				QueryParams
			);

			DrawDebugLine(
				GetWorld(),
				StartPoint,
				StartPoint + RotatedVector,
				bHit == true ? FColor::Blue : FColor::Red,
				false,  // 지속적으로 그릴 것인지 여부
				1.0f,   // 지속 시간
				0,      // DepthPriority
				1.0f    // 선의 두께
			);
		}

	}
}

