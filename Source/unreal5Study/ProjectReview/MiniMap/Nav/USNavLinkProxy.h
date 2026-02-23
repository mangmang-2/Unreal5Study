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

USTRUCT(BlueprintType)
struct FNavLinkJumpConfig
{
    GENERATED_BODY()

    // LaunchCharacter에 넘길 속도 벡터 (로컬 방향 기준)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
    FVector LaunchVelocity = FVector(300.f, 0.f, 600.f);

    // LaunchVelocity를 월드 기준으로 쓸지, 링크 액터 로컬 기준으로 쓸지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
    bool bUseLocalDirection = true;

    // 점프 시 캐릭터 회전값 (Yaw만 실질적으로 사용됨)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
    FRotator TargetRotation = FRotator(0.f, 0.f, 0.f);

    // XY 속도 오버라이드 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
    bool bOverrideXY = true;

    // Z 속도 오버라이드 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
    bool bOverrideZ = true;
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
    UFUNCTION()
    void ResumeMovement(class ACharacter* Character, const FVector& Destination);

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

    // 링크별 점프 설정 (에디터에서 직접 세팅)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NavLink|Jump Config")
    FNavLinkJumpConfig JumpConfig;

    // 발사 방향 화살표
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavLink|Debug")
    class UArrowComponent* LaunchArrow;

    EMovementMode SaveMovementMode;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    void UpdateLaunchArrow();
#endif
};
