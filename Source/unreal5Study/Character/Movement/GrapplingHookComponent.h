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
	void HookStart();
	void HookEnd();

	void SwingStart();
	void SwingAction(float DeltaTime);

	void CameraTargeting(float DeltaTime, float AlignSpeed);

	void HookProgress();
	void HookRelrease();
	bool TargetTest();
	void HookStart(FVector GrabPoint);
	void OwnerTurn(FVector GrabPoint);
	void HookEndPostion(FVector GrabPoint);
	
	void SetHookState(bool bState);
	void HookAction();

	void OnGrappleAimUpdate(FVector Target, bool bIsValidGrapplePoint);
		
protected:
	
	bool bIsGrappling = false;
	FVector GrabHookPoint;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> NSGrapple;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> NiagaraSystemAsset;

	EHookState HookState = EHookState::None;

	UPROPERTY()
	TObjectPtr<class USphereComponent> HookTempPoint = nullptr;
	float TimeAccumulator = 0.f;       // 시간 누적용

	float InitialSwingRadius = 0.0f;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RopeActorClass;

	TObjectPtr<AActor> RopeActorInstance;

	void AttachRope();

};
