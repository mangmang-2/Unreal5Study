// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/USEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

AUSEnemyAIController::AUSEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Perception component ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	// �ð� ���� ��� �߰�
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 800.0f; // Ž�� �Ÿ�
	SightConfig->LoseSightRadius = 800; // Ž�� ���� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 45.0f; // �þ� ��  �̷��� �ؾ� 90��
	SightConfig->SetMaxAge(1.0f); // OnPerceptionUpdated ������Ʈ �ð�
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 0;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // �� ����
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // �߸� ����
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // �Ʊ� ����

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AUSEnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AUSEnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AUSEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AUSEnemyAIController::OnPerceptionUpdated);

}

void AUSEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//RunAI();
}

void AUSEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUSEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Log, TEXT("OnPerceptionUpdated"));

	for (AActor* Actor : UpdatedActors)
	{
		if (Actor == nullptr)
		{
			continue;
		}

		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(Actor, Info);

		for (FAIStimulus Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				APawn* PawnActor = Cast<APawn>(Actor);
				if (PawnActor && PawnActor->GetController() && PawnActor->GetController()->IsPlayerController())
				{
					PerceptionActors.Add(Actor);
					UE_LOG(LogTemp, Log, TEXT("WasSuccessfullySensed"));
				}
			}
			else
			{
				APawn* PawnActor = Cast<APawn>(Actor);
				if (PawnActor && PawnActor->GetController()->IsPlayerController())
				{
					PerceptionActors.Remove(Actor);
					UE_LOG(LogTemp, Log, TEXT("Not WasSuccessfullySensed"));
				}
			}
		}
	}
}

float AUSEnemyAIController::GetRadius()
{
	return SightConfig->SightRadius;
}

float AUSEnemyAIController::GetAngleDegrees()
{
	return SightConfig->PeripheralVisionAngleDegrees;
}
