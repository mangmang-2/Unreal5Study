// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/AI/BTTaskNode_TargetLocation.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "../../Interface/USCharacterAIInterface.h"

EBTNodeResult::Type UBTTaskNode_TargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
		return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
		return EBTNodeResult::Failed;


	AActor* CurrentTargetObject = Cast<AActor>(Blackboard->GetValueAsObject(Target.SelectedKeyName));
	if (CurrentTargetObject == nullptr)
		return EBTNodeResult::Failed;

	IUSCharacterAIInterface* TargetActor = Cast<IUSCharacterAIInterface>(CurrentTargetObject);
	if (TargetActor == nullptr)
		return EBTNodeResult::Failed;

	float TargetRadius = TargetActor->GetAICollisionRange();
	FVector Center = CurrentTargetObject->GetActorLocation();
	FVector PawnLoc = Pawn->GetActorLocation();

	FVector Dir = PawnLoc - Center;
	Dir = Dir.GetSafeNormal();
	FVector FinalLocation = Center + Dir * TargetRadius;

	Blackboard->SetValueAsVector(TargetLocation.SelectedKeyName, FinalLocation);

	return EBTNodeResult::Succeeded;
}

void UBTTaskNode_TargetLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = Asset.GetBlackboardAsset();
	if (BBAsset)
	{
		Target.ResolveSelectedKey(*BBAsset);
	}
}
