// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Movement/USClimbingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"

// Sets default values for this component's properties
UUSClimbingComponent::UUSClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
}


// Called when the game starts
void UUSClimbingComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
	
}


// Called every frame
void UUSClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsClimbingFalling || bIsClimbingMontage)
		return;

	// 클라이밍 상태
	if (bIsClimbing == false)
	{
		if (ClimbingStart() == false)
			return;
	}

	ClimbingLocation(DeltaTime);

}

bool UUSClimbingComponent::ClimbingStart()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return false;

	if (owner->GetCharacterMovement()->IsFalling())
	{
		FVector ForwardVector = owner->GetCapsuleComponent()->GetForwardVector() * 100;
		FVector StartPoint = owner->GetCapsuleComponent()->GetComponentLocation();

		FVector MiddleEndPoint = StartPoint + ForwardVector;
		FHitResult HitResultMiddle;
		bool bHitMiddle = HitCheck(StartPoint, MiddleEndPoint, HitResultMiddle, true, -1.0f, false);

		if (bHitMiddle)
		{
			bIsClimbing = true;
			owner->GetMovementComponent()->Velocity = FVector(0, 0, 0);

			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(owner->GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->bOrientRotationToMovement = false;
				CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
			}

			float CapsuleRadius = owner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 0;
			owner->SetActorLocation(HitResultMiddle.Normal * CapsuleRadius + HitResultMiddle.Location);

			FRotator Rotation = FRotationMatrix::MakeFromX(HitResultMiddle.Normal * -1).Rotator();
			owner->SetActorRotation(Rotation);
		}
	}

	return bIsClimbing;
}

void UUSClimbingComponent::ClimbingLocation(float DeltaTime)
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;
	if (bIsClimbing)
	{
		FVector ForwardVector = owner->GetCapsuleComponent()->GetForwardVector() * 100;
		FVector StartPoint = owner->GetCapsuleComponent()->GetComponentLocation();

		FVector MiddleEndPoint = StartPoint + ForwardVector;
		FHitResult HitResultMiddle;
		bool bHitMiddle = HitCheck(StartPoint, MiddleEndPoint, HitResultMiddle, true, -1.0f, false);
		if (bHitMiddle)
		{

			FRotator Rotation = FRotationMatrix::MakeFromX(HitResultMiddle.Normal * -1).Rotator();
			/*if (TargetRotation == Rotation)
				return;*/

			//TargetRotation = Rotation;
			float CapsuleRadius = owner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 0;
			owner->SetActorLocation(HitResultMiddle.Normal * CapsuleRadius + HitResultMiddle.Location);

			owner->SetActorRotation(Rotation);
		}
		
	}
}

bool UUSClimbingComponent::GetHeadPoint(FHitResult& HitResult)
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return false;

	if (bIsClimbing == false)
		return false;

	//// 머리 확인
	FVector UpVector = owner->GetCapsuleComponent()->GetUpVector() * 60;
	FVector StartPoint = owner->GetCapsuleComponent()->GetComponentLocation();
	FVector ForwardVector = owner->GetCapsuleComponent()->GetForwardVector() * 80;

	FVector HeadStartPoint = StartPoint + UpVector;
	FVector HeadEndPoint = StartPoint + ForwardVector + UpVector;
	FHitResult HitResultHead;

	bool bHitHead = HitCheck(HeadStartPoint, HeadEndPoint, HitResultHead, false, -1.0f, false);
	if (bHitHead == false)
	{
		FVector Offset = owner->GetCapsuleComponent()->GetForwardVector() * 10;
		FVector OffsetStart = HeadStartPoint - UpVector;
		for (int i = 0; i < 9; ++i)
		{
			FVector SPoint = OffsetStart + Offset * i;
			FVector EPoint = SPoint + UpVector * 3;

			if (HitCheck(EPoint, SPoint, HitResult, false, -1.0f, false))
			{
				return true;
			}
		}
	}
	return false;
}

void UUSClimbingComponent::ClimbingUp()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	FHitResult HitResult;
	if (GetHeadPoint(HitResult))
	{
		// 캐릭터가 들어갈 수 있는 곳인지 확인
		float CapsuleRadius = owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FVector CapsuleOrigin = HitResult.ImpactPoint + owner->GetCapsuleComponent()->GetUpVector() * (CapsuleHalfHeight + 1);
		FHitResult CapsuleRadiusHitResult;
		if (CapsuleHitCheck(CapsuleOrigin, CapsuleRadius, CapsuleHalfHeight, CapsuleRadiusHitResult) == false)
		{
			if(MotionWarping)
				MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("Warp1"), HitResult.ImpactPoint);

			UAnimInstance* AnimInstance = owner->GetMesh()->GetAnimInstance();
			if (AnimInstance && ClimbingTopMontage && bIsClimbingMontage == false)
			{
				bIsClimbing = false;
				bIsClimbingMontage = true;
				AnimInstance->Montage_Play(ClimbingTopMontage, 1.0);

				float MontageLength = ClimbingTopMontage->GetPlayLength();

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
					ClimbingClear();
					})
					, MontageLength, false);
			} 
		}
	}
}

void UUSClimbingComponent::ClimbingClear()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	if (GetWorld())
	{
		bIsClimbingFalling = true;
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			bIsClimbingFalling = false;
			})
			, 1.0, false);
	}

	bIsClimbing = false;
	bIsClimbingMontage = false;
	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(owner->GetMovementComponent());
	if (CharMoveComp)
	{
		CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		CharMoveComp->bOrientRotationToMovement = true;
	}
}

void UUSClimbingComponent::ClimbingCornerLeft()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	FVector UpVector = owner->GetCapsuleComponent()->GetUpVector() * 60;
	FVector ForwardVector = owner->GetCapsuleComponent()->GetForwardVector() * 80;
	FVector StartPoint = owner->GetCapsuleComponent()->GetComponentLocation();
	FVector LeftVector = owner->GetCapsuleComponent()->GetRightVector() * -60;

	ClimbingCorner(StartPoint, StartPoint + LeftVector * 2, ClimbingTurnCornerLMontage);
	ClimbingOutSideCorner(StartPoint, StartPoint + LeftVector * 2, owner->GetCapsuleComponent()->GetForwardVector() * 100, ClimbingOutSideTurnCornerLMontage);
}

void UUSClimbingComponent::ClimbingCornerRight()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	FVector UpVector = owner->GetCapsuleComponent()->GetUpVector() * 60;
	FVector ForwardVector = owner->GetCapsuleComponent()->GetForwardVector() * 80;
	FVector StartPoint = owner->GetCapsuleComponent()->GetComponentLocation();
	FVector LeftVector = owner->GetCapsuleComponent()->GetRightVector() * -60;
	ClimbingCorner(StartPoint, StartPoint - LeftVector * 2, ClimbingTurnCornerRMontage);
	ClimbingOutSideCorner(StartPoint, StartPoint - LeftVector * 2, owner->GetCapsuleComponent()->GetForwardVector() * 100, ClimbingOutSideTurnCornerRMontage);
}

void UUSClimbingComponent::ClimbingCorner(FVector StartPoint, FVector EndPoint, UAnimMontage* Montage)
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	FHitResult HitResult;
	bool bHit = HitCheck(StartPoint, EndPoint, HitResult, false, -1.0f, false);
	if (bHit)
	{
		float CapsuleRadius = owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector CapsuleOrigin = HitResult.ImpactPoint + HitResult.Normal * (CapsuleRadius + 1);
		FHitResult CapsuleRadiusHitResult;
		if (CapsuleHitCheck(CapsuleOrigin, CapsuleRadius, CapsuleHalfHeight, CapsuleRadiusHitResult) == false)
		{
			UAnimInstance* AnimInstance = owner->GetMesh()->GetAnimInstance();
			if (AnimInstance && Montage && bIsClimbing && bIsClimbingMontage == false)
			{
				bIsClimbingMontage = true;
				FVector LookDirection = -HitResult.Normal;
				MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("TurnCorner"), HitResult.ImpactPoint + HitResult.Normal * (CapsuleRadius), FRotationMatrix::MakeFromX(LookDirection).Rotator());

				AnimInstance->Montage_Play(Montage, 1.0);
				float MontageLength = Montage->GetPlayLength();

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
					bIsClimbingMontage = false;
					})
					, MontageLength, false);
			}
		}
	}
}

void UUSClimbingComponent::ClimbingOutSideCorner(FVector StartPoint, FVector EndPoint, FVector OffSet, UAnimMontage* Montage)
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	FVector SPoint = StartPoint + OffSet;
	FVector EPoint = EndPoint + OffSet;

	FHitResult HitResult;
	if (HitCheck(EPoint, SPoint, HitResult, false, -1.0f, false))
	{
		float CapsuleRadius = owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector CapsuleOrigin = HitResult.ImpactPoint + HitResult.ImpactNormal * (CapsuleRadius + 1);
		FHitResult CapsuleRadiusHitResult;
		if (CapsuleHitCheck(CapsuleOrigin, CapsuleRadius, CapsuleHalfHeight, CapsuleRadiusHitResult) == false)
		{
			UAnimInstance* AnimInstance = owner->GetMesh()->GetAnimInstance();
			if (AnimInstance && Montage && bIsClimbing && bIsClimbingMontage == false)
			{
				bIsClimbingMontage = true;
				FVector LookDirection = -HitResult.Normal;
				MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("OutSide1"), EndPoint);
				MotionWarping->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("OutSide2"), HitResult.ImpactPoint, LookDirection.Rotation());

				AnimInstance->Montage_Play(Montage, 1.0);
				float MontageLength = Montage->GetPlayLength();
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
					bIsClimbingMontage = false;
					})
					, MontageLength, false);
			}
		}
	}
}

bool UUSClimbingComponent::HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage)
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

bool UUSClimbingComponent::CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult)
{
	if (GetOwner() == nullptr)
		return false;

	const FVector StartPoint = CapsuleOrigin;
	const FVector EndPoint = StartPoint;
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);  // 캡슐 크기 설정: 반지름, 높이

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner()); // 이 액터는 트레이스에서 제외

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		FQuat::Identity,  // 회전 없음
		ECC_PhysicsBody,  // 충돌 채널
		Capsule,
		QueryParams
	);

	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//FColor DrawColor = bHit == true ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(GetOwner()->GetActorUpVector()).ToQuat(), DrawColor, false);

	return bHit;
}

