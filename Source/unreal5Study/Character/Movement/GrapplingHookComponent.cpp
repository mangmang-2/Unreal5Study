// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Movement/GrapplingHookComponent.h"
#include "../USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/CapsuleComponent.h"

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
			NSGrapple->SetAsset(NiagaraSystemAsset);
			NSGrapple->Activate(false);
			NSGrapple->Deactivate();
			HookState = EHookState::None;
		}
	}

	GrappleAnchorPoint = NewObject<USphereComponent>(this, TEXT("GrappleHookPoint"));
	GrappleAnchorPoint->RegisterComponent();

}

// Called every frame
void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HookState == EHookState::Aim)
	{
		if(TargetTest())
		{
			GrappleNiagaraUpdate(GrabHookPoint, true);
		}
		else
		{
			GrappleNiagaraUpdate(GrabHookPoint, false);
		}
	}
	else if (HookState == EHookState::Progress)
	{
		HookAction();
	}
	else if (HookState == EHookState::Swing)
	{
		float TargetLength = 400.0f;
		float InterpSpeed = 1.0f;

		CurrentLimitedLength = FMath::FInterpTo(CurrentLimitedLength, TargetLength, DeltaTime, InterpSpeed);
		SetLimitedLength(CurrentLimitedLength);
	}
}

void UGrapplingHookComponent::BeginAim()
{
	HookState = EHookState::Aim;
	NSGrapple->Activate(true);
}

void UGrapplingHookComponent::EndAim()
{
	if(IsAiming() == false)
		return;

	HookState = EHookState::None;
	NSGrapple->Activate(false);
	NSGrapple->Deactivate();

	SwingStart();
}

bool UGrapplingHookComponent::TargetTest()
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return false;

	UCameraComponent* CameraComponent = Owner->GetFollowCamera();
	FVector CameraLocation = CameraComponent->GetComponentLocation();
	FVector ForwardVector = CameraComponent->GetForwardVector();

	FHitResult HitResult;
	FVector Start = CameraLocation;
	FVector End = CameraLocation + (ForwardVector * TraceDistance);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	Params.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		GrabHookPoint = HitResult.ImpactPoint;
	}
	else
	{
		GrabHookPoint = End; // 시선 끝 지점
	}

	return bHit;
}

void UGrapplingHookComponent::GrappleNiagaraUpdate(FVector Target, bool bIsValidGrapplePoint)
{
	if (NSGrapple == nullptr)
		return;

	FName BeamEndVarName = TEXT("Beam End");
	NSGrapple->SetVariableVec3(BeamEndVarName, Target);

	FName ValidTargetVarName = TEXT("Valid Target");
	float ValidValue = bIsValidGrapplePoint ? 1.0f : 0.0f;
	NSGrapple->SetVariableFloat(ValidTargetVarName, ValidValue);
}

bool UGrapplingHookComponent::IsRopeAction()
{
	return HookState != EHookState::None;
}

bool UGrapplingHookComponent::IsAiming()
{
	return HookState == EHookState::Aim;
}

void UGrapplingHookComponent::SwingEnd()
{
	SwingStop();
}

void UGrapplingHookComponent::SwingStart()
{
	TimeAccumulator = 0.0f;
	HookState = EHookState::Swing;

	HookEndPostion(GrabHookPoint);
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	AttachRope();
}

void UGrapplingHookComponent::SwingStop()
{
	HookState = EHookState::None;

	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	UCapsuleComponent* Capsule = Owner->GetCapsuleComponent();

	if (Capsule)
	{
		FVector Velocity = Owner->GetVelocity();
		Capsule->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Owner->LaunchCharacter(Velocity, true, true);
	}

	if (RopeActorInstance != nullptr)
	{
		RopeActorInstance->Destroy();
	}
}

void UGrapplingHookComponent::HookStart()
{
	SwingStop();
	HookState = EHookState::Progress;
}

void UGrapplingHookComponent::HookEndPostion(FVector GrabPoint)
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	GrappleAnchorPoint->SetWorldLocation(GrabPoint);
}

void UGrapplingHookComponent::HookAction()
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

void UGrapplingHookComponent::SetLimitedLength(float LimitedLength)
{
	UPhysicsConstraintComponent* PhysicsConstraint = Cast<UPhysicsConstraintComponent>(RopeActorInstance->GetDefaultSubobjectByName(TEXT("PhysicsConstraint")));
	if (PhysicsConstraint)
	{
		PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, LimitedLength);
		PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, LimitedLength);
		PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, LimitedLength);
	}
}

void UGrapplingHookComponent::StopAction()
{
	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	HookState = EHookState::None;
	if (NSGrapple)
	{
		NSGrapple->Activate(false);
		NSGrapple->Deactivate();
	}

	UCapsuleComponent* Capsule = Owner->GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (RopeActorInstance != nullptr)
	{
		RopeActorInstance->Destroy();
	}
}

void UGrapplingHookComponent::AttachRope()
{
	if (RopeActorClass == nullptr || GetOwner() == nullptr)
		return;

	AUSPlayer* Owner = Cast<AUSPlayer>(GetOwner());
	if (Owner == nullptr)
		return;

	UWorld* World = GetWorld();
	if (World == nullptr) 
		return;

	if (RopeActorInstance != nullptr)
	{
		RopeActorInstance->Destroy();
	}

	RopeActorInstance = World->SpawnActor<AActor>(RopeActorClass);

	if (RopeActorInstance == nullptr) 
		return;

	RopeActorInstance->SetActorLocation(GrabHookPoint);

	UStaticMeshComponent* CableEnd = Cast<UStaticMeshComponent>(RopeActorInstance->GetDefaultSubobjectByName(TEXT("CableEnd")));
	UCapsuleComponent* Capsule = Owner->GetCapsuleComponent();

	if(CableEnd && Capsule)
	{
		CableEnd->SetWorldLocation(Owner->GetActorLocation() + FVector(0.f, 0.f, 0));		

		Capsule->AttachToComponent(
			CableEnd,
			FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				EAttachmentRule::KeepWorld,
				true
			)
		);
	}

	CurrentLimitedLength = (GrabHookPoint - Owner->GetActorLocation()).Length();
	SetLimitedLength(CurrentLimitedLength);
}

void UGrapplingHookComponent::HookEnd()
{
	HookEndPostion(GrabHookPoint);

	HookState = EHookState::Progress;
	NSGrapple->Deactivate();
}
