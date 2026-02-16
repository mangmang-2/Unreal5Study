// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Object/USMiniMapComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UUSMiniMapComponent::UUSMiniMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCapture2D"));
	SceneCapture->SetupAttachment(this);

	// 기본 세팅
	SceneCapture->bCaptureEveryFrame = false;
	SceneCapture->bCaptureOnMovement = false;
	SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
}


// Called when the game starts
void UUSMiniMapComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
		return;

	SavedBaseLocation = Owner->GetActorLocation();
	bHasSavedBase = true;

	if (RenderTarget)
	{
		SceneCapture->TextureTarget = RenderTarget;
	}

	SceneCapture->OrthoWidth = OrthoWidth;
	UpdateCaptureTransform();
	DoCapture();
}


// Called every frame
void UUSMiniMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UpdateCaptureTransform();

	Accum += DeltaTime;
	if (CaptureInterval > 0.0f && Accum >= CaptureInterval)
	{
		Accum = 0.f;
		DoCapture();
	}
}

void UUSMiniMapComponent::UpdateCaptureTransform()
{
	if (SceneCapture == nullptr)
		return;

	const FRotator Rot(-90.0f, 90.0f, 0.0f);

	SceneCapture->SetWorldLocationAndRotation(GetPos(), Rot);
	SceneCapture->OrthoWidth = OrthoWidth;
}

void UUSMiniMapComponent::DoCapture()
{
	if (SceneCapture)
	{
		SceneCapture->CaptureScene();
	}
}

void UUSMiniMapComponent::AddPan(FVector2D Delta)
{
	// 이동속도
	float MoveAmount = 20.0f;
	PanOffset += (Delta * MoveAmount);

	PanOffset.X = FMath::Clamp(PanOffset.X, -MaxPanDistance, MaxPanDistance);
	PanOffset.Y = FMath::Clamp(PanOffset.Y, -MaxPanDistance, MaxPanDistance);

	UpdateCaptureTransform();
	DoCapture();
}

void UUSMiniMapComponent::SetZoom(float DeltaZoom)
{
	OrthoWidth -= DeltaZoom;

	OrthoWidth = FMath::Clamp(OrthoWidth, MinZoomWidth, MaxZoomWidth);
	SceneCapture->OrthoWidth = OrthoWidth;
	DoCapture();
}

void UUSMiniMapComponent::ResetPan()
{
	PanOffset = FVector2D::ZeroVector;

	UpdateCaptureTransform();
	DoCapture();
}

FVector UUSMiniMapComponent::GetPos()
{
	AActor* Owner = GetOwner();
	if (Owner == nullptr)
		return FVector::ZeroVector;

	if (bHasSavedBase == false)
	{
		SavedBaseLocation = Owner->GetActorLocation();
		bHasSavedBase = true;
	}

	const FVector Loc = SavedBaseLocation + FVector(PanOffset.X, PanOffset.Y, Height);

	return Loc;
}
