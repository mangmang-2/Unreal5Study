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

	// Perception component 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	// 시각 감지 기능 추가
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 800.0f; // 탐지 거리
	SightConfig->LoseSightRadius = 800; // 탐지 제한 거리
	SightConfig->PeripheralVisionAngleDegrees = 45.0f; // 시야 각  이렇게 해야 90도
	SightConfig->SetMaxAge(1.0f); // OnPerceptionUpdated 업데이트 시간
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 0;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 감지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 감지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 아군 감지

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

	DrawSightCone();
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
void AUSEnemyAIController::DrawSightCone()
{
	if (SightConfig && GetPawn())
	{
		float Radius = SightConfig->SightRadius;
		float HalfAngle = SightConfig->PeripheralVisionAngleDegrees;

		int32 NumberOfSections = ceilf(HalfAngle * 2.0f / 1.0f);
		float DAngle = HalfAngle * 2.0f / NumberOfSections;
		FVector StartPoint = GetPawn()->GetActorLocation();
		StartPoint.Z -= 90;
		TArray<FHitResult> HitArray;
		for (int32 i = 0; i < NumberOfSections; i++)
		{
			int32 L_CurrentAngle = DAngle* i - HalfAngle;
		
			// 회전 축
			FVector RotationAxis(0.0f, 0.0f, 1.0f);

			// 회전 각도 (도 단위)
			float RotationAngle = L_CurrentAngle;

			// 벡터 회전
			FVector RotatedVector = GetPawn()->GetActorForwardVector().RotateAngleAxis(RotationAngle, RotationAxis) * Radius;
			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			QueryParams.AddIgnoredActor(GetPawn()); // 이 액터는 트레이스에서 제외
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
				-1,   // 지속 시간
				0,      // DepthPriority
				1.0f    // 선의 두께
			);
		}

	}
}
