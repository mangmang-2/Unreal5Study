// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/AI/BTTaskNode_FindNearest.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "../../Interface/USCharacterAIInterface.h"

EBTNodeResult::Type UBTTaskNode_FindNearest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;

	// Target.SelectedKeyName
	// Blackboard->SetValueAsObject(Target.SelectedKeyName, object)
	
	AActor* TargetTownObject = UGameplayStatics::GetActorOfClass(GetWorld(), TargetTownClass);
	if(TargetTownObject == nullptr)
		return EBTNodeResult::Failed;

	const float PawnToTownDist = FVector::Distance(Pawn->GetActorLocation(), TargetTownObject->GetActorLocation());

	if(PawnToTownDist < 100.0f)
		return EBTNodeResult::Succeeded;

	float BestDistance = PawnToTownDist;
	AActor* BestActor = TargetTownObject;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTagName, FoundActors);
	FoundActors.Add(TargetTownObject);
	for (const auto& Actor : FoundActors)
	{
		if (Actor == nullptr) 
			continue;

		float DistToPawn = FVector::Distance(Actor->GetActorLocation(), Pawn->GetActorLocation());

		if(DistToPawn > 300)
			continue;

		// 가장 가까운 액터를 추적
		if (DistToPawn < BestDistance)
		{
			BestDistance = DistToPawn;
			BestActor = Actor;
		}
	}
	AActor* CurrentTargetObject = Cast<AActor>(Blackboard->GetValueAsObject(Target.SelectedKeyName));
	if (CurrentTargetObject != nullptr)
	{
		if (BestActor == CurrentTargetObject)
		{
			return EBTNodeResult::Succeeded;
		}
	}

	IUSCharacterAIInterface* BestActorAI = Cast<IUSCharacterAIInterface>(BestActor);
	if (BestActorAI == nullptr)
		return EBTNodeResult::Failed;

	float TargetRadius = BestActorAI->GetAICollisionRange();
	FVector Center = BestActor->GetActorLocation();
	FVector PawnLoc = Pawn->GetActorLocation();

	FVector Dir = PawnLoc - Center;
	Dir = Dir.GetSafeNormal();
	FVector FinalLocation = Center + Dir * TargetRadius;

	Blackboard->SetValueAsObject(Target.SelectedKeyName, BestActor);
	Blackboard->SetValueAsVector(TargetLocation.SelectedKeyName, FinalLocation);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_FindNearest::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (BBAsset)
	{
		TargetClass.ResolveSelectedKey(*BBAsset);
		Target.ResolveSelectedKey(*BBAsset);
	}
}
