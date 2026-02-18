// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "USNavLinkProxy.generated.h"

/**
 * NavLink 액션 타입
 */
UENUM(BlueprintType)
enum class ENavLinkActionType : uint8
{
	None    UMETA(DisplayName = "None"),
	Vault   UMETA(DisplayName = "Vault"),   // 낮은 장애물 넘기
	Jump    UMETA(DisplayName = "Jump"),    // 점프
	Climb   UMETA(DisplayName = "Climb"),   // 등반
	Drop    UMETA(DisplayName = "Drop"),    // 낙하
	Slope   UMETA(DisplayName = "Slope"),   // 경사로 (링크 생성 안함)
};


/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:
    AUSNavLinkProxy();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NavLink")
    ENavLinkActionType ActionType = ENavLinkActionType::Jump;
protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnLinkReached(class AActor* MovingActor, const FVector& DestinationPoint);
    
    void HandleVault(class ACharacter* Character, const FVector& Destination);
    void HandleJump(class ACharacter* Character, const FVector& Destination);
    void HandleClimb(class ACharacter* Character, const FVector& Destination);
    void HandleDrop(class ACharacter* Character, const FVector& Destination);

    // 동작 완료 후 이동 재개
    void ResumeMovement(class AAIController* AIC, const FVector& Destination);

    void DrawDebugLink();

public:
    void SetupSmartLink(const FVector& Start, const FVector& End);
    UFUNCTION()
    void OnTriggerOverlap(
        UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NavLink")
	TObjectPtr<UStaticMeshComponent> TriggerMeshLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NavLink")
	TObjectPtr<UStaticMeshComponent> TriggerMeshRight;

    UPROPERTY()
    bool bIsJumping = false;

    UPROPERTY()
    FVector TargetJumpDirection;

    UPROPERTY()
    ACharacter* JumpingCharacter = nullptr;
};
