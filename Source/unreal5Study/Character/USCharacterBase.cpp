// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MotionWarpingComponent.h"
#include "Engine/DamageEvents.h"
#include "ProceduralMeshComponent.h"
#include "Movement/USClimbingComponent.h"
#include "AbilitySystemComponent.h"
#include "Ability/Tag/USGameplayTag.h"
#include "USPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/USCharacterAnimInstance.h"
#include "ModularCharacter/USModularCharacterComponent.h"
#include "Movement/USParkourComponent.h"
#include "Movement/GrapplingHookComponent.h"
#include "CableComponent.h"

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
		MoveComp->JumpZVelocity = 310.f;
		MoveComp->AirControl = 0.35f;
		MoveComp->MaxWalkSpeed = 500.f;
		MoveComp->MinAnalogWalkSpeed = 20.f;
		MoveComp->BrakingDecelerationWalking = 2000.f;
		MoveComp->BrakingDecelerationFalling = 1500.0f;
	}


	// Mesh
	auto MeshComp = GetMesh();
	if(MeshComp)
	{
		MeshComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
		MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	EquipSword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipSword"));
	EquipSword->SetupAttachment(GetMesh(), TEXT("HandSocket_R"));
	EquipSword->SetCollisionProfileName(TEXT("NoCollision"));
	EquipSword->SetIsReplicated(true);

	EquipShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipShield"));
	EquipShield->SetupAttachment(GetMesh(), TEXT("HandSocket_L"));
	EquipShield->SetCollisionProfileName(TEXT("NoCollision"));
	EquipShield->SetIsReplicated(true);

	//ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
	ParkourComponent = CreateDefaultSubobject<UUSParkourComponent>(TEXT("ParkourComponent"));
	ClimbingComponent = CreateDefaultSubobject<UUSClimbingComponent>(TEXT("ClimbingComponent"));

	ModularCharacterComponent = CreateDefaultSubobject<UUSModularCharacterComponent>(TEXT("ModularCharacterComponent"));
	
	GrapplingHookComponent = CreateDefaultSubobject<UGrapplingHookComponent>(TEXT("GrapplingHookComponent"));
}

// Called when the game starts or when spawned
void AUSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	ShowSword(bIsCombatState);
	ShowShield(bIsCombatState);

	if (ClimbingComponent)
		ClimbingComponent->SetClimb(false);

	if (UUSCharacterAnimInstance* Anim = Cast<UUSCharacterAnimInstance>(GetMesh()->GetAnimInstance()) ; Anim != nullptr && ASCComponent != nullptr)
	{
		Anim->InitializeWithAbilitySystem(ASCComponent);
	}
}

// Called every frame
void AUSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	QueryParams.AddIgnoredActor(this); // �� ���ʹ� Ʈ���̽����� ����
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Pawn,
		QueryParams
	);

	// ���� Ʈ���̽� ��θ� ����׿����� �׸���
	if (DrawLine)
	{
		if (bHit)
		{
			DrawDebugLine(
				GetWorld(),
				StartPoint,
				EndPoint,
				FColor::Blue,
				false,  // ���������� �׸� ������ ����
				DrawLineTime,   // ���� �ð�
				0,      // DepthPriority
				1.0f    // ���� �β�
			);
		}
		else
		{
			DrawDebugLine(
				GetWorld(),
				StartPoint,
				EndPoint,
				FColor::Red,
				false,  // ���������� �׸� ������ ����
				DrawLineTime,   // ���� �ð�
				0,      // DepthPriority
				1.0f    // ���� �β�
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
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);  // ĸ�� ũ�� ����: ������, ����

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); // �� ���ʹ� Ʈ���̽����� ����

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		FQuat::Identity,  // ȸ�� ����
		ECC_PhysicsBody,  // �浹 ä��
		Capsule,
		QueryParams
	);

	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	FColor DrawColor = bHit == true ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, CapsuleRadius, FRotationMatrix::MakeFromZ(GetActorUpVector()).ToQuat(), DrawColor, false);

	return bHit;
}

bool AUSCharacterBase::IsClimbing()
{
	if(ClimbingComponent == nullptr)
		return false;

	return ClimbingComponent->IsClimbing();
}

bool AUSCharacterBase::IsClimbingMontage()
{
	if (ClimbingComponent == nullptr)
		return false;

	return ClimbingComponent->IsClimbingMontage();
}

bool AUSCharacterBase::IsClimbingFalling()
{
	if (ClimbingComponent == nullptr)
		return false;

	return ClimbingComponent->IsClimbingFalling();
}

void AUSCharacterBase::ShowSword(bool bShow)
{
	if (EquipSword == nullptr)
		return;

	if(bShow)
		EquipSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandSocket_R"));
	else
		EquipSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sword_Holder"));

}

void AUSCharacterBase::ShowShield(bool bShow)
{
	if (EquipShield == nullptr)
		return;

	if (bShow)
		EquipShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HandSocket_L"));
	else
		EquipShield->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Shield_Holder"));
}

void AUSCharacterBase::StopGrapplingHook()
{
	if (GrapplingHookComponent)
	{
		GrapplingHookComponent->StopAction();
	}
}

UAbilitySystemComponent* AUSCharacterBase::GetAbilitySystemComponent() const
{
	return ASCComponent;
}

void AUSCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AUSPlayerState* USPlayerState = GetPlayerState<AUSPlayerState>();
	if (USPlayerState == nullptr)
		return;
	ASCComponent = USPlayerState->GetAbilitySystemComponent();
	ASCComponent->InitAbilityActorInfo(USPlayerState, this);
	ASCComponent->SetIsReplicated(true);

	int32 AbilityCount = 0 ;
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		StartSpec.InputID = AbilityCount++;
		ASCComponent->GiveAbility(StartSpec);
	}
}

void AUSCharacterBase::SetDeathEvent()
{
	FGameplayEventData EventData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, USTAG_INPUT_STATE_DEATH, EventData);
}

bool AUSCharacterBase::IsHookAim()
{
	return GrapplingHookComponent->IsRopeAction();
}

FVector AUSCharacterBase::GetRopePoint()
{
	return GrapplingHookComponent->GetGrabHookPoint();
}