// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "USCharacterBase.generated.h"

UCLASS()
class UNREAL5STUDY_API AUSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;


public:
	// 추후에 애님 인스턴스로 이동하거나 데이터를 줘야할듯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingEdge : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingUp : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTopMontage;

public:
	bool HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DebugMessage);
	bool CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult);
	void ClimbingClear();

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMotionWarpingComponent> MotionWarping;
};
