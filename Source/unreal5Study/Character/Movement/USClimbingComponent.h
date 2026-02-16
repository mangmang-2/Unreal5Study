// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USClimbingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UUSClimbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSClimbingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// 추후에 애님 인스턴스로 이동하거나 데이터를 줘야할듯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingMontage : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsClimbingFalling : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTopMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTurnCornerLMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingTurnCornerRMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingOutSideTurnCornerLMontage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ClimbingOutSideTurnCornerRMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<class UMotionWarpingComponent> MotionWarping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bClimbOn : 1;

	float ForwardDis = 100.0f;
	FVector CachedWallNormal = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetWallDistance = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LocationInterpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RotationInterpSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWallAngleChange = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxWallDepthChange = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TraceDistance = 150.0f;

    FVector PreviousWallNormal = FVector::ZeroVector;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NormalSampleRadius = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NormalSampleGridSize = 3; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NormalSmoothingStrength = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseSphereTrace = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SphereTraceRadius = 20.0f;
public:
	bool ClimbingStart();
	void ClimbingLocation(float DeltaTime);
	bool TraceForWall(FHitResult& OutHit, FVector& OutWallNormal);
	bool ShouldExitClimbing(const FVector& CurrentWallNormal, float WallDistance);

	bool GetHeadPoint(FHitResult& HitResult);
	void ClimbingUp();

	void ClimbingClear();

	void ClimbingCornerLeft();
	void ClimbingCornerRight();

	void ClimbingCorner(FVector StartPoint, FVector EndPoint, UAnimMontage* Montage);
	void ClimbingOutSideCorner(FVector StartPoint, FVector EndPoint, FVector OffSet, UAnimMontage* Montage);

	bool IsClimbing() { return bIsClimbing; }
	bool IsClimbingMontage() { return bIsClimbingMontage; }
	bool IsClimbingFalling() { return bIsClimbingFalling; }

	void SetClimb(bool bOn) { bClimbOn = bOn; }
public:
	bool HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage);
	bool CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult);

};
