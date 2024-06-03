// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTService_Detect.h"
#include "BTService_Detect.h"
#include "AIController.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "USEnemyAIController.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
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

	float DetectRadius = AIPawn->GetAIDetectRange();

	for (auto& Actor : AIController->PerceptionActors)
	{
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
