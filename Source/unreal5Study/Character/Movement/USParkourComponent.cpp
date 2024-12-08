// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Movement/USParkourComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "../USCharacterBase.h"

// Sets default values for this component's properties
UUSParkourComponent::UUSParkourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

//	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping1"));
}


// Called when the game starts
void UUSParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UUSParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//IsParkourable();
}

bool UUSParkourComponent::IsParkourable()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return false;

	UCapsuleComponent* Capsule = owner->GetCapsuleComponent();
	if (Capsule == nullptr)
		return false;

	FVector ForwardVector = Capsule->GetForwardVector();
	FVector UpVector = Capsule->GetUpVector();
	FVector OwnerPoint = Capsule->GetComponentLocation();

	FHitResult HitResult;

	FVector Start1 = OwnerPoint;
	FVector End1 = OwnerPoint + ForwardVector * 100;
	if (HitCheck(Start1, End1, HitResult, true, -1.0f, false))
	{
		return false;
	}

	FVector Start2 = OwnerPoint - UpVector * 20;
	FVector End2 = OwnerPoint - UpVector * 20 + ForwardVector * 100;
	if (HitCheck(Start2, End2, HitResult, true, -1.0f, false))
	{
		return true;
	}


	return false;
}

void UUSParkourComponent::Start()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;
	UAnimInstance* AnimInstance = owner->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
		return;
	if (OverObstaclesMontage == nullptr)
		return;

	FVector StartPoint, MiddlePoint, EndPoint;
	GetHitPoint(StartPoint, MiddlePoint, EndPoint);

	if(StartPoint == FVector::ZeroVector || MiddlePoint == FVector::ZeroVector || EndPoint == FVector::ZeroVector)
		return;
	
	AUSCharacterBase* CharacterBase = Cast<AUSCharacterBase>(owner);
	UMotionWarpingComponent* MotionWarping = CharacterBase->GetMotionWarping();
	if (MotionWarping)
	{
		UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(owner->GetMovementComponent());
		if (CharMoveComp)
		{
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
		}
		owner->SetActorEnableCollision(false);

		MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("start"), StartPoint);
		MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("middle"), MiddlePoint);
		MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("end"), EndPoint);
	}
	AnimInstance->Montage_Play(OverObstaclesMontage);

	float MontageLength = OverObstaclesMontage->GetPlayLength();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		ACharacter* owner = Cast<ACharacter>(GetOwner());
		if (owner == nullptr)
			return;
		UAnimInstance* AnimInstance = owner->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
			return;
		if (OverObstaclesMontage == nullptr)
			return;
		UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(owner->GetMovementComponent());
		if (CharMoveComp)
		{
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
			owner->SetActorEnableCollision(true);
		}
		})
		, MontageLength, false);

}

void UUSParkourComponent::GetHitPoint(FVector& StartPoint, FVector& MiddlePoint, FVector& EndPoint)
{
	StartPoint = FVector::ZeroVector;
	MiddlePoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	UCapsuleComponent* Capsule = owner->GetCapsuleComponent();
	if (Capsule == nullptr)
		return;

	FVector ForwardVector = Capsule->GetForwardVector();
	FVector UpVector = Capsule->GetUpVector();
	FVector OwnerPoint = Capsule->GetComponentLocation();


	for (int32 i = 0; i < 10; i++)
	{
		FHitResult HitResult;
		FVector Start = OwnerPoint + UpVector * 100 + ForwardVector * ForwardSize * i;
		FVector End = OwnerPoint - UpVector * 70 + ForwardVector * ForwardSize * i;
		if (HitCheck(Start, End, HitResult, true, -1.0f, false))
		{
			if(StartPoint == FVector::ZeroVector)
				StartPoint = HitResult.Location;

			MiddlePoint = HitResult.Location;
		}
		else if (StartPoint != FVector::ZeroVector)
		{
			if (EndPoint == FVector::ZeroVector)
			{
				Start = OwnerPoint + UpVector * 100 + ForwardVector * ForwardSize * (i + 2);
				End = OwnerPoint - UpVector * 200 + ForwardVector * ForwardSize * (i + 2);
				if (HitCheck(Start, End, HitResult, true, -1.0f, false))
				{
					EndPoint = HitResult.Location;
					break;
				}
				
			}
		}
	}

	DrawDebugSphere(GetWorld(), StartPoint, 10.0f, 16, FColor::Red, false, 1.0f, 0, 2.0f);
	DrawDebugSphere(GetWorld(), MiddlePoint, 10.0f, 16, FColor::Green, false, 1.0f, 0, 2.0f);
	DrawDebugSphere(GetWorld(), EndPoint, 10.0f, 16, FColor::Blue, false, 1.0f, 0, 2.0f);

}

bool UUSParkourComponent::HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage)
{
	if (GetOwner() == nullptr)
		return false;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner()); // 이 액터는 트레이스에서 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Pawn,
		QueryParams
	);

	// 라인 트레이스 경로를 디버그용으로 그리기
	if (DrawLine)
	{
		if (bHit)
		{
			DrawDebugLine(
				GetWorld(),
				StartPoint,
				EndPoint,
				FColor::Blue,
				false,  // 지속적으로 그릴 것인지 여부
				DrawLineTime,   // 지속 시간
				0,      // DepthPriority
				1.0f    // 선의 두께
			);
		}
		else
		{
			DrawDebugLine(
				GetWorld(),
				StartPoint,
				EndPoint,
				FColor::Red,
				false,  // 지속적으로 그릴 것인지 여부
				DrawLineTime,   // 지속 시간
				0,      // DepthPriority
				1.0f    // 선의 두께
			);
		}
	}


	if (DebugMessage)
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Hit: %s"), bHit ? *FString("true") : *FString("false")));

	return bHit;
}