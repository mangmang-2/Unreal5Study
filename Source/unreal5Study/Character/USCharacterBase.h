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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingCorner : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTurnCornerLMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTurnCornerRMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingOutSideTurnCornerLMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingOutSideTurnCornerRMontage;


public:
	bool HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage);
	bool CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult);
	void ClimbingClear();

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UMotionWarpingComponent> MotionWarping;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ImpactMontage;

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

public:
	void NormalAttack();
	bool WeaponAttackCheck(TSet<AActor*>& HitActors);
	bool AttackCheck(TSet<AActor*>& HitActors);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void ComboActionBegin();
	void ComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();
	int32 GetMaxCombo();

	virtual void NotifyComboActionEnd();

public:
	float HalfAngle = 45.0;
	float DeltaAngle = 1.0;
	float ExternalRadius = 1500.0;

	UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UProceduralMeshComponent> ProceduralMesh;


};
