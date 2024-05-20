// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AUSCharacterBase::AUSCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	auto MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		MoveComp->JumpZVelocity = 500.f;
		MoveComp->AirControl = 0.35f;
		MoveComp->MaxWalkSpeed = 500.f;
		MoveComp->MinAnalogWalkSpeed = 20.f;
		MoveComp->BrakingDecelerationWalking = 2000.f;
	}

	// Mesh
	auto MeshComp = GetMesh();
	if(MeshComp)
	{
		MeshComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
		MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("HandSocket_R"));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	ClimbingClear();
}

// Called when the game starts or when spawned
void AUSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG



#endif
}

// Called to bind functionality to input
void AUSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AUSCharacterBase::HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage)
{
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); // 이 액터는 트레이스에서 제외
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

bool AUSCharacterBase::CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult)
{
	const FVector StartPoint = CapsuleOrigin;
	const FVector EndPoint = StartPoint;
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);  // 캡슐 크기 설정: 반지름, 높이

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); // 이 액터는 트레이스에서 제외

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
	FColor DrawColor = bHit == true ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(GetActorUpVector()).ToQuat(), DrawColor, false);

	return bHit;
}

void AUSCharacterBase::ClimbingClear()
{
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
	bIsClimbingUp = false;
	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharMoveComp)
	{
		CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		CharMoveComp->bOrientRotationToMovement = true;
	}
}

void AUSCharacterBase::NormalAttack()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	if (bIsClimbingFalling || bIsClimbingUp || bIsClimbingCorner || bIsAttack)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && NormalAttackMontage)
	{
		bIsAttack = true;
		AnimInstance->Montage_Play(NormalAttackMontage, 1.0);

		UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (CharMoveComp)
		{
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_None);
		}

		// 이건 나중에 쓸떄가 있으니.. 뼈대만 남겨두고
		float MontageLength = NormalAttackMontage->GetPlayLength();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
				bIsAttack = false;
			}
			})
			, MontageLength, false);
	}
}

bool AUSCharacterBase::WeaponAttackCheck(TSet<AActor*>& HitActors)
{
	if (Weapon)
	{
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(this); // 이 액터는 트레이스에서 제외

		const FVector Start = Weapon->GetComponentLocation();
		const FVector End = Weapon->GetSocketLocation("EndPoint");

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Pawn,
			QueryParams
		);


		FColor DrawColor = bHit && HitActors.Contains(HitResult.GetActor()) == false ? FColor::Green : FColor::Red;

		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Blue,
			false,  // 지속적으로 그릴 것인지 여부
			-1.0f,   // 지속 시간
			0,      // DepthPriority
			1.0f    // 선의 두께
		);


		if (bHit && HitActors.Contains(HitResult.GetActor()) == false)
		{
			HitActors.Add(HitResult.GetActor());
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(0, DamageEvent, GetController(), this);
		}

	}
	return false;
}

bool AUSCharacterBase::AttackCheck(TSet<AActor*>& HitActors)
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 100.0f;
	const float AttackRadius = 20.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius), Params);

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected && HitActors.Contains(OutHitResult.GetActor()) == false ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif

	if (HitDetected && HitActors.Contains(OutHitResult.GetActor()) == false)
	{
		HitActors.Add(OutHitResult.GetActor());
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}



	return HitDetected;
}

float AUSCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	AUSCharacterBase* USCharacterBase = Cast<AUSCharacterBase>(DamageCauser);
	if (USCharacterBase && USCharacterBase->GetCapsuleComponent())
	{
		FVector ForwardVector = USCharacterBase->GetCapsuleComponent()->GetForwardVector();
		//ForwardVector.Z = 0;
		ForwardVector *= 200;

		LaunchCharacter(ForwardVector, false, false);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ImpactMontage)
	{
		AnimInstance->Montage_Play(ImpactMontage, 1.0);
	}

	return DamageAmount;
}
