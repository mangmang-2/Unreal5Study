// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USCropoutPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/DecalComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Interactable/USResourceInterface.h"
#include "../UI/USCropoutWidgetComponent.h"
#include "../Stat/USCropoutStat.h"

// Sets default values
AUSCropoutPawn::AUSCropoutPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VillagerCapsule"));
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerHat"));
	Hat->SetupAttachment(SkeletalMesh, TEXT("headSocket"));

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerHair"));
	Hair->SetupAttachment(SkeletalMesh);

	Tool = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VillagerTool"));
	Tool->SetupAttachment(SkeletalMesh, TEXT("hand_rSocket"));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("VillagerMovement"));

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("VillagerDecal"));
	Decal->SetupAttachment(SkeletalMesh);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UUSCropoutStat>(TEXT("AttributeSet"));

	HPBarWidgetComponent = CreateDefaultSubobject<UUSCropoutWidgetComponent>(TEXT("Widget"));
	HPBarWidgetComponent->SetupAttachment(SkeletalMesh);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 240.0f));
}

// Called when the game starts or when spawned
void AUSCropoutPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSCropoutPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUSCropoutPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AUSCropoutPawn::Action(class AActor* VillagerAction)
{
	if (VillagerAction == nullptr)
		return;

	TargetRef = VillagerAction;

	if (TargetRef && TargetRef->IsValidLowLevel())
	{
		if (TargetRef->Tags.IsValidIndex(0))
		{
			// 태그가 유효하면 작업 변경 함수 호출
			FName NewJob = TargetRef->Tags[0];
			ChangeJob(NewJob);
			UpdateAllPawn();
		}
	}
}



void AUSCropoutPawn::ChangeJob(FName NewJob)
{
	UDataTable* JobDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Cropout/Player/DT_Jobs.DT_Jobs"));
	if (JobDataTable == nullptr)
	{
		return;
	}

	FSTJob* JobRow = JobDataTable->FindRow<FSTJob>(NewJob, TEXT("Change Job"));
	if (JobRow == nullptr)
	{
		return;
	}

	if (Tags.Contains(NewJob))
		return;

	if (Tags.Num() > 1)
	{
		Tags[0] = NewJob;
	}
	else
	{
		Tags.Insert(NewJob, 0);
	}

	ResetJobState();

	BehaviorTreeLoaded(JobRow->BehaviourTree);
	LoadAndStoreAccessories(JobRow);
}

void AUSCropoutPawn::UpdateAllPawn()
{

}


void AUSCropoutPawn::ResetJobState()
{
	StopJob();
	if (Hat)
	{
		Hat->SetSkeletalMesh(nullptr);
	}

	if (Tool)
	{
		Tool->SetStaticMesh(nullptr);
	}

	if (TargetTool)
	{
		TargetTool = nullptr;
	}
}

void AUSCropoutPawn::StopJob()
{
	if (SkeletalMesh)
	{
		UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->StopAllMontages(0.0f);
		}
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement();
	}


	Quantity = 0;
}


void AUSCropoutPawn::BehaviorTreeLoaded(class UBehaviorTree* BehaviorTree)
{
	if (BehaviorTree == nullptr)
		return;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	if (AIController->RunBehaviorTree(BehaviorTree) == false)
		return;

	ActiveBehavior = BehaviorTree;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (Blackboard)
	{
		FName KeyName = "Target";

		Blackboard->SetValueAsObject(KeyName, TargetRef);
	}
}

void AUSCropoutPawn::LoadAndStoreAccessories(FSTJob* Job)
{
	UAnimMontage* AnimMontage = Cast<UAnimMontage>(Job->WorkAnim);
	if (AnimMontage)
	{
		WorkAnim = AnimMontage;
	}

	USkeletalMesh* HatMesh = Cast<USkeletalMesh>(Job->Hat);
	if (HatMesh)
	{
		Hat->SetSkeletalMesh(HatMesh);
	}

	TargetTool = Cast<UStaticMesh>(Job->Tool);
	if (Tool && TargetTool)
	{
		Tool->SetStaticMesh(TargetTool);
	}
}




void AUSCropoutPawn::PlayVillagerAnim(UAnimMontage* Montage, float Length)
{
	if (SkeletalMesh == nullptr)
		return;
	if (Montage == nullptr)
		return;

	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	// 몽타주 재생
	float MontageLength = AnimInstance->Montage_Play(Montage, 1.0f); // 재생 속도 1.0

	if (MontageLength > 0.0f)
	{
		// Length 값이 유효하지 않다면 몽타주의 길이를 사용
		float DelayDuration = (Length > 0.0f) ? Length : MontageLength;

		FTimerHandle MontageDelayHandle;
		GetWorldTimerManager().SetTimer(
			MontageDelayHandle,
			this,
			&AUSCropoutPawn::OnMontageComplete,
			DelayDuration,
			false
		);
	}
}

void AUSCropoutPawn::OnMontageComplete()
{
	if (SkeletalMesh == nullptr)
		return;
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;

	AnimInstance->StopAllMontages(0.0f);
}


