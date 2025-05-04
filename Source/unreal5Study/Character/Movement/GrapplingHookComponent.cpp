// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Movement/GrapplingHookComponent.h"
#include "../USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"
#include "../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

// Sets default values for this component's properties
UGrapplingHookComponent::UGrapplingHookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();

	if (NSGrapple == nullptr && NiagaraSystemAsset)
	{
		// NiagaraComponent 생성
		NSGrapple = NewObject<UNiagaraComponent>(GetOwner());
		if (NSGrapple)
		{
			NSGrapple->RegisterComponent(); // 반드시 등록
			NSGrapple->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			NSGrapple->SetAsset(NiagaraSystemAsset); // 미리 UPROPERTY로 NiagaraSystem 참조 받아둬야 함
			NSGrapple->SetRelativeLocation(FVector::ZeroVector);
			NSGrapple->Activate(true);
		}
	}
	
}

void UGrapplingHookComponent::HookStart()
{
	HookState = EHookState::Aim;
	NSGrapple->Activate(true);
}

void UGrapplingHookComponent::HookEnd()
{
	//OwnerTurn(GrabHookPoint);
	HookStart(GrabHookPoint);
	//HookAction();
	//SetHookState(false);
	HookRelrease();

	HookState = EHookState::Progress;
	NSGrapple->Deactivate();
}

void UGrapplingHookComponent::CameraTargeting(float DeltaTime, float AlignSpeed)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	UCameraComponent* FollowCamera = Owner->GetFollowCamera();

	if (!FollowCamera) return;

	// 카메라의 정면 방향 (Z축 제외)
	FVector CameraForward = FollowCamera->GetForwardVector().GetSafeNormal2D();
	FRotator TargetRotation = CameraForward.Rotation(); // Yaw만 필요

	FRotator CurrentRotation = Owner->GetActorRotation();

	// 부드럽게 보간 (Yaw만 적용)
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, AlignSpeed);
	NewRotation.Pitch = 0.0f;
	NewRotation.Roll = 0.0f;

	Owner->SetActorRotation(NewRotation);
}

// Called every frame
void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HookState == EHookState::Aim)
	{
		TargetTest();
		OnGrappleAimUpdate(GrabHookPoint, true);
		CameraTargeting(DeltaTime, 5.0f);
	}
	if (HookState == EHookState::Progress)
	{
		HookAction();
	}

	//HookEndPostion(GrabHookPoint);
}

void UGrapplingHookComponent::HookProgress()
{
	//if (TargetTest())
	//{
	//	//OwnerTurn(GrabHookPoint);
	//	HookStart(GrabHookPoint);	
	//}
}

void UGrapplingHookComponent::HookRelrease()
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;
	UCableComponent* CableComponent = Owner->GetGrapplingCable();
	if (CableComponent == nullptr)
		return;

	CableComponent->bAttachEnd = false;
}

bool UGrapplingHookComponent::TargetTest()
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if(Owner == nullptr)
		return false;

	UCameraComponent* CameraComponent =  Owner->GetFollowCamera();
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector ForwardVector = CameraComponent->GetForwardVector();
	float TraceDistance = 30000.0f;

	FHitResult HitResult;
	FVector Start = CameraLocation;
	FVector End = CameraLocation + (ForwardVector * TraceDistance);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner); // 자기 자신 무시
	Params.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	/*DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Red,
		false,
		1.0f,
		0,
		1.0f
	);*/
	if (bHit)
	{
		//DrawDebugSphere(
		//	GetWorld(),
		//	HitResult.ImpactPoint,     // 히트 지점
		//	16.0f,                     // 반지름
		//	12,                        // 세그먼트 수 (둥글기)
		//	FColor::Green,             // 색상
		//	false,                     // 영구 여부 (true면 안 사라짐)
		//	2.0f                       // 지속 시간 (초 단위)
		//);

		GrabHookPoint = HitResult.ImpactPoint;
	}
	
	return bHit;
}

void UGrapplingHookComponent::HookStart(FVector GrabPoint)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;
	UCableComponent* CableComponent = Owner->GetGrapplingCable();
	if (CableComponent == nullptr)
		return;

	CableComponent->bAttachEnd = true;
	HookEndPostion(GrabPoint);

	SetHookState(true);
}

void UGrapplingHookComponent::OwnerTurn(FVector GrabPoint)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	FVector OwnerLocation = Owner->GetActorLocation();
	FVector Direction = (GrabPoint - OwnerLocation).GetSafeNormal();

	FRotator TargetRotation = Direction.Rotation();

	FRotator NewRotation = Owner->GetActorRotation();
	NewRotation.Yaw = TargetRotation.Yaw;
	//Owner->AddActorWorldRotation(NewRotation);
	Owner->SetActorRotation(NewRotation);
	{
		FVector Start = Owner->GetActorLocation();
		FVector ForwardVector = NewRotation.Vector(); // 회전값을 방향 벡터로 변환
		FVector End = Start + ForwardVector * 500.0f; // 500만큼 앞으로 선

		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Green,
			false,
			2.0f,
			0,
			2.0f
		);
	}
}

void UGrapplingHookComponent::HookEndPostion(FVector GrabPoint)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	UCableComponent* CableComponent = Owner->GetGrapplingCable();
	if (CableComponent == nullptr)
		return;

	FTransform ActorTransform = Owner->GetActorTransform();
	FVector LocalLocation = ActorTransform.InverseTransformPosition(GrabHookPoint);

	CableComponent->EndLocation = LocalLocation;
}

void UGrapplingHookComponent::SetHookState(bool bState)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
	if (CharMoveComp == nullptr)
		return;

	bIsGrappling = bState;

	/*if (bState)
	{
		CharMoveComp->bOrientRotationToMovement = false;
		CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);

		CharMoveComp->bEnablePhysicsInteraction = true;

	}
	else
	{
		CharMoveComp->bOrientRotationToMovement = true;
		CharMoveComp->SetMovementMode(EMovementMode::MOVE_Falling);
	}*/
}

void UGrapplingHookComponent::HookAction()
{
	if (bIsGrappling)
	{
		AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
		if (Owner == nullptr)
			return;

		FVector ActorLocation = Owner->GetActorLocation();
		FVector LaunchVelocity = (GrabHookPoint - ActorLocation).GetSafeNormal() * 1500.0f;
		Owner->LaunchCharacter(LaunchVelocity, true, true);

		const float DistanceToTarget = FVector::Dist(ActorLocation, GrabHookPoint);

		if (DistanceToTarget < 150.0f) // 도착 거리 기준 (적당히 조정)
		{
			HookState = EHookState::None;
		}
	}
}

void UGrapplingHookComponent::OnGrappleAimUpdate(FVector Target, bool bIsValidGrapplePoint)
{
	if (NSGrapple == nullptr) 
		return;

	FName BeamEndVarName = TEXT("Beam End");
	NSGrapple->SetVariableVec3(BeamEndVarName, Target);

	FName ValidTargetVarName = TEXT("Valid Target");
	float ValidValue = bIsValidGrapplePoint ? 1.0f : 0.0f;
	NSGrapple->SetVariableFloat(ValidTargetVarName, ValidValue);
}

