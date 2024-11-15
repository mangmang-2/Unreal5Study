// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "USCropoutPlayer.generated.h"

UCLASS()
class UNREAL5STUDY_API AUSCropoutPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUSCropoutPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateZoom();
	void Dof();

	void MoveTracking();
	void EdgeMode();
	void ProjectMouseToGroundPlane(FVector2D& ScreenPosition, FVector& Intersection, bool& bMousePostion);
	FVector2D GetViewportCenter();
	FVector2D GetMouseViewportPosition(bool& bMousePostion);
	FVector ProjectScreenPositionToGamePlane(FVector2D ScreenPosition);

	FVector CursorDistFromViewportCenter(FVector2D ScreenPos);
	FVector2D CalculateEdgeMoveDistance();
	FVector2D OffsetMousePositionToCreateDeadZone(FVector2D ScreenPos, FVector2D Distance);
	FVector2D AdjustForNegativeDirection(FVector2D InputVector);

	void PositionCollisionOnGroundPlane();
	void UpdateCursorPosition();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

	UFUNCTION()
	void ClearHoverActor();

	UFUNCTION()
	void ClosestHoverCheck();

	void FindClosestOverlappingActor();

	UFUNCTION(BlueprintCallable)
	void BlueprintZoomFunc(float ActionValue);

	UFUNCTION(BlueprintCallable)
	void BlueprintVillagerModeTriggered();

	UFUNCTION(BlueprintCallable)
	void BlueprintVillagerModeStarted();
	UFUNCTION(BlueprintCallable)
	void BlueprintVillagerModeComplete();

	void VillagerRelease();

	UFUNCTION(BlueprintCallable)
	void BlueprintDragModeTriggered();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> BaseInputMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> VillagerMappingContext;

		UPROPERTY(EditAnywhere)
	TObjectPtr<class UInputMappingContext> DragMoveMappingContext;


	UPROPERTY(EditAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCurveFloat> ZoomCurve;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> Collision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Cursor;

	float ZoomDirection = 0.0f;
	float ZoomValue = 0.5;
	float EdgeMoveDistance = 50.0f;

	FTransform Target;

	UPROPERTY()
	TObjectPtr<class AActor> HoverActor;
	FTimerHandle TimerHandle_ClosestHoverCheck;

	UPROPERTY()
	TObjectPtr<class AActor> VillagerAction;

private:
	FVector TargetHandle;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> NSPath;

	UPROPERTY()
	TObjectPtr<class AActor> Selected;

	FTimerHandle UpdatePathTimerHandle;;

	FVector StoredMove;
public:
	void PositionCheck();
	class AActor* VillageOverlapCheck();
	void VillagerSelect(class AActor* SelectedActor);

	UFUNCTION()
	void UpdatePath();

	void TrackerMove();
	FVector CalculateCameraOffset();
};
