// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Movement/GrapplingHookComponent.h"
#include "../USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"

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

	// ...
	
}


// Called every frame
void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HookEndPostion(GrabHookPoint);
}

void UGrapplingHookComponent::HookProgress()
{
	if (TargetTest())
	{
		//OwnerTurn(GrabHookPoint);
		HookStart(GrabHookPoint);	
	}
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
	Params.AddIgnoredActor(Owner); // �ڱ� �ڽ� ����
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
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,     // ��Ʈ ����
			16.0f,                     // ������
			12,                        // ���׸�Ʈ �� (�ձ۱�)
			FColor::Green,             // ����
			false,                     // ���� ���� (true�� �� �����)
			2.0f                       // ���� �ð� (�� ����)
		);

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
		FVector ForwardVector = NewRotation.Vector(); // ȸ������ ���� ���ͷ� ��ȯ
		FVector End = Start + ForwardVector * 500.0f; // 500��ŭ ������ ��

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

void UGrapplingHookComponent::HookEnd()
{
	OwnerTurn(GrabHookPoint);
	HookAction();
	SetHookState(false);
	HookRelrease();
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
		FVector DirectionToGrab = (GrabHookPoint - ActorLocation).GetSafeNormal(); // ���� ���� ����

		float MoveRightLeftInput = Owner->GetInputAxisValue("MoveRight");
		FVector RightVector = Owner->GetActorRightVector();
		FVector MoveOffset = RightVector * MoveRightLeftInput * 0.7;

		FVector CombinedDirection = (DirectionToGrab + MoveOffset).GetSafeNormal();

		FVector TargetLocation = CombinedDirection * 2000;
		//Owner->GetCharacterMovement()->AddForce(TargetLocation);

		FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();
		DrawDebugLine(GetWorld(), ActorLocation, TargetLocation, FColor::Red, false, 2.0f);
		//bIsGrappling = false;

		Owner->GetCharacterMovement()->AddImpulse(TargetLocation, true);
	}
}

