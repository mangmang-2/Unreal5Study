// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USBuildConfirm.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Border.h"
#include "Kismet/KismetMathLibrary.h"
#include <Cropout/Player/USCropoutPlayer.h>
#include <Cropout/Interactable/USInteractable.h>

void UUSBuildConfirm::NativeConstruct()
{
    Super::NativeConstruct();
}

void UUSBuildConfirm::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateBorderPosition(InDeltaTime); 
}

FVector2D UUSBuildConfirm::GetClampedScreenPosition()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController == nullptr || PlayerController->GetPawn() == nullptr)
        return FVector2D::ZeroVector;

    AUSCropoutPlayer* TargetActor = Cast<AUSCropoutPlayer>(PlayerController->GetPawn());
    if (TargetActor == nullptr)
        return FVector2D::ZeroVector;

    FVector WorldPosition = TargetActor->TargetSpawn->GetActorLocation();
    FVector2D ScreenPosition;
    PlayerController->ProjectWorldLocationToScreen(WorldPosition, ScreenPosition, true);

    FVector2D ViewportSize;
    GEngine->GameViewport->GetViewportSize(ViewportSize);

    float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(PlayerController);

    ScreenPosition /= ViewportScale;

    ViewportSize /= ViewportScale;


    FVector2D ReturnPosition;
    ReturnPosition.X = FMath::Clamp(ScreenPosition.X, 150.0f, ViewportSize.X - 150.0f);
    ReturnPosition.Y = FMath::Clamp(ScreenPosition.Y, 0.0f, ViewportSize.Y - 350.0f);

    return ReturnPosition;
}

void UUSBuildConfirm::UpdateBorderPosition(float InDeltaTime)
{
    if (Border == nullptr)
        return;

    FVector2D TargetPosition = GetClampedScreenPosition();
    FVector TargetPosition2 = FVector(TargetPosition.X, TargetPosition.Y, 0);

    FVector CurrentPosition(Border->RenderTransform.Translation.X, Border->RenderTransform.Translation.Y, 0.0);

    // Spring Interpolation (Vector Spring Interp)
    FVectorSpringState SpringState;
    float Stiffness = 50.0f;
    float CriticalDampingFactor = 0.9f;
    float Mass = 1.0f;
    float TargetVelocityAmount = 0.75f;

    
    FVector InterpolatedPosition = UKismetMathLibrary::VectorSpringInterp(
        CurrentPosition,                // 현재 위치
        TargetPosition2,                // 목표 위치
        SpringState,                    // 스프링 상태 (캐시 변수)
        Stiffness,                      // 강성 (Stiffness)
        CriticalDampingFactor,          // 댐핑 비율
        InDeltaTime * 20,   // 델타 타임
        Mass,                           // 질량
        TargetVelocityAmount            // 목표 속도 비율
    );

    // Border의 위치 업데이트
    FWidgetTransform Transform;
    Transform.Translation = FVector2D(InterpolatedPosition.X, InterpolatedPosition.Y);
    Transform.Scale = FVector2D(1.0f, 1.0f);
    Transform.Shear = FVector2D(0.0f, 0.0f);
    Transform.Angle = 0.0f;

    // 변환 설정
    Border->SetRenderTransform(Transform);
}