// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/USCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../USCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "../USPlayer.h"

UUSCharacterAnimInstance::UUSCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UUSCharacterAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	GameplayTagPropertyMap.Initialize(this, ASC);
}

void UUSCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}

	OwnerChracterBase = Cast<AUSCharacterBase>(GetOwningActor());
	bIsClimbing = 0;
}

void UUSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);


		if (OwnerChracterBase)
		{
			bIsClimbing = OwnerChracterBase->IsClimbing();
			bIsClimbingMontage = OwnerChracterBase->IsClimbingMontage();
			bIsHookAim = OwnerChracterBase->IsHookAim();
			RopeEndPoint = OwnerChracterBase->GetRopePoint();
			bToAirMotion = OwnerChracterBase->IsToAirMotion();
			DistanceToGround = Movement->CurrentFloor.FloorDist;

		}

		if (Cast<AUSPlayer>(GetOwningActor()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("bIsFalling%d"), bIsFalling);
			//UE_LOG(LogTemp, Warning, TEXT("DistanceToGround %f"), DistanceToGround);

			//UE_LOG(LogTemp, Warning, TEXT("bToAirMotion%d"), bToAirMotion);
			//UE_LOG(LogTemp, Warning, TEXT("bIsJumping%d"), bIsJumping);

			//UE_LOG(LogTemp, Warning, TEXT("%s"), *Movement->GetMovementName());
		}

		ClimbingUP = Velocity.Z * 100.0;

		if (Owner)
		{
			RightVelocity = FVector::DotProduct(Velocity, Owner->GetActorRightVector());
			ForwardVelocity = FVector::DotProduct(Velocity, Owner->GetActorForwardVector());

			ClimbingRight = RightVelocity * 100.0;
		}

		
		//UE_LOG(LogTemp, Warning, TEXT("Climbing%s"), *Velocity.ToString());
		//if (OwnerChracterBase->bIsClimbingEdge) // 가장 자리에 도달한 순간 모션은 종료되어야함
		//{
		//	ClimbingUP = 0;
		//	ClimbingRight = 0;
		//}
	}
}
