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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UMotionWarpingComponent> MotionWarping;

public:
	bool ClimbingStart();
	void ClimbingLocation(float DeltaTime);

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

public:
	bool HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage);
	bool CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult);
};
