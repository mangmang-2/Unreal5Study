// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/USMiniMapComponent.h"
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
	if (!Owner) return;

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
	if (CaptureInterval > 0.f && Accum >= CaptureInterval)
	{
		Accum = 0.f;
		DoCapture();
	}
}

void UUSMiniMapComponent::UpdateCaptureTransform()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	const FVector OwnerLoc = Owner->GetActorLocation();
	const float Yaw = bRotateWithOwnerYaw ? Owner->GetActorRotation().Yaw : 0.f;

	// 위에서 아래로 내려다보는 회전: Pitch -90
	const FRotator Rot(-90.f, Yaw, 0.f);
	const FVector Loc = OwnerLoc + FVector(0, 0, Height);

	SceneCapture->SetWorldLocationAndRotation(Loc, Rot);
	SceneCapture->OrthoWidth = OrthoWidth;

}

void UUSMiniMapComponent::DoCapture()
{
	if (SceneCapture)
	{
		SceneCapture->CaptureScene();
	}
}

