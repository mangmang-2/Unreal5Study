// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrapplingHookComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UGrapplingHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	enum EHookState
	{
		None,
		Aim,
		TargetOn,
		Progress,
		Swing,
	};
public:	
	// Sets default values for this component's properties
	UGrapplingHookComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BeginAim();
	void EndAim();

	bool TargetTest();
	void GrappleNiagaraUpdate(FVector Target, bool bIsValidGrapplePoint);

	bool IsRopeAction();
	bool IsAiming();
	void SwingEnd();

	void SwingStart();
	void SwingStop();

	void HookStart();
	void AttachRope();
	void HookEnd();
	void HookEndPostion(FVector GrabPoint);
	void HookAction();

	void SetLimitedLength(float LimitedLength);

	void StopAction();

protected:

	EHookState HookState = EHookState::None;
	float TraceDistance = 3000.0f;
	FVector GrabHookPoint;
	float TimeAccumulator = 0.f;       // 시간 누적용
	float InitialSwingRadius = 0.0f;

	float CurrentLimitedLength;
	
	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> NSGrapple;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> NiagaraSystemAsset;

	// 로프를 걸기 위한 장소
	UPROPERTY()
	TObjectPtr<class USphereComponent> GrappleAnchorPoint = nullptr;
	
	// 로프 액터
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> RopeActorClass;

	UPROPERTY()
	TObjectPtr<class AActor> RopeActorInstance;
};
