// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/TextureRenderTarget.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
AUSInteractable::AUSInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    RootComponent = Scene;

    // Static Mesh ������Ʈ ���� �� ��Ʈ�� ����
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Scene); // SceneRoot�� ����

    // Box Collision ������Ʈ ���� �� ��Ʈ�� ����
    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(Scene); // SceneRoot�� ����

    WobbleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("WobbleTimeline"));


}

// Called when the game starts or when spawned
void AUSInteractable::BeginPlay()
{
	Super::BeginPlay();
	
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this; // �ݹ� ���
    LatentInfo.ExecutionFunction = "AfterDelay"; // ���� �� ȣ���� �Լ� �̸�
    LatentInfo.Linkage = 0; // Latent Action�� ���� ID
    LatentInfo.UUID = 1; // Unique ID

    // DelayUntilNextTick ȣ��
    UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);

    if (WobbleCurve)
    {
        FOnTimelineFloat TimelineUpdate;
        TimelineUpdate.BindUFunction(this, FName("HandleTimelineUpdate"));
        WobbleTimeline->AddInterpFloat(WobbleCurve, TimelineUpdate);
        WobbleTimeline->SetLooping(true);
    }
}

void AUSInteractable::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FVector Min, Max;
    Mesh->GetLocalBounds(Min, Max);
    Max /= 100.0f;

    auto RoundToGrid = [this](float Value) -> float {
        return FMath::RoundToFloat(Value) * 100.0f;
        };

    FVector RoundedMax = FVector(
        RoundToGrid(Max.X),
        RoundToGrid(Max.Y),
        RoundToGrid(Max.Z));

    // �浹 �ڽ��� �߽ɰ� ũ�� ���
    float Max1 = FMath::Max(FMath::Max(RoundedMax.X, RoundedMax.Y), 100.0f);
    float Max2 = FMath::Max(RoundedMax.Z, 100.0f);

    
    FVector BoxExtent(Max1, Max1, Max2);
    float BoundGap = 0.0f;
    
    Box->SetBoxExtent(BoxExtent + (BoundGap * 100.0f));
    FRotator Rotation = FRotator::MakeFromEuler(FVector(1.0f, 0.0f, 0.0f));
    Box->SetWorldRotation(Rotation);
}

void AUSInteractable::Destroyed()
{
    Super::Destroyed();
    
    if (EnableGroundBlend == false)
        return;

    if (RTDraw == nullptr || RenderMaterial == nullptr)
        return;

    UKismetRenderingLibrary::ClearRenderTarget2D(this, RTDraw);
}

void AUSInteractable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Timeline ������Ʈ
   // WobbleTimeline->TickTimeline(DeltaTime);
}

void AUSInteractable::DoDestroy()
{
    FVector NewLocation(10000.0f, 10000.0f, 10000.0f);
    SetActorLocation(NewLocation);
    Destroy();
}

void AUSInteractable::PlacementMode()
{
    if (MeshList.IsValidIndex(0))
    {
        Mesh->SetStaticMesh(MeshList[0]);
    }

    Tags.Empty();
    Tags.Add(TEXT("PlacementMode"));
}
void AUSInteractable::AfterDelay()
{
    WriteToRenderTarget();
    CheckForOverlappingActors();
}

void AUSInteractable::WriteToRenderTarget()
{
    if (EnableGroundBlend == false)
        return;

    if (RTDraw == nullptr || RenderMaterial == nullptr)
        return;

    UCanvas* Canvas;
    FVector2D Size;
    FDrawToRenderTargetContext Context;

    // ĵ���� �ʱ�ȭ �� ���� Ÿ�� �غ�
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RTDraw, Canvas, Size, Context);

    if (Canvas)
    {
        FVector2D ScreenPosition;
        FVector2D ScreenSize;
        TransformToTexture(Size, ScreenPosition, ScreenSize);

        FVector2D CoordinatePosition = FVector2D::ZeroVector;
        FVector2D CoordinateSize(1.0f, 1.0f);
        float Rotation = 0.0f;
        FVector2D PivotPoint(0.5f, 0.5f);
        
        Canvas->K2_DrawMaterial(RenderMaterial, ScreenPosition, ScreenSize, CoordinatePosition, CoordinateSize, Rotation, PivotPoint);

        UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
    }
}

void AUSInteractable::TransformToTexture(FVector2D Size, FVector2D& ReturnValue, FVector2D& ReturnValue2)
{
    FVector Origin, BoxExtent;
    GetActorBounds(false, Origin, BoxExtent);
    float OutlineDrawValue = FMath::Min(BoxExtent.X, BoxExtent.Y) / 10000.0f * Size.X * OutlineDraw;
    ReturnValue2.X = OutlineDrawValue;
    ReturnValue2.Y = OutlineDrawValue;

    ReturnValue = FVector2D((GetActorLocation() + 10000.0) / 20000.0f * Size.X - (OutlineDrawValue / 2.0f));
    return;
}

void AUSInteractable::CheckForOverlappingActors()
{
    // ��ġ�� ���͸� ã�� ���� �迭
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, AUSInteractable::StaticClass()); // BP_Interactable Ŭ������ �ش��ϴ� ���� ã��

    // ��� ��ġ�� ���Ϳ� ���� �˻�
    for (AActor* OverlappingActor : OverlappingActors)
    {
        if (OverlappingActor && OverlappingActor != this)
        {
            FVector OtherLocation = OverlappingActor->GetActorLocation();
            FVector SelfLocation = GetActorLocation();
            bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(OtherLocation, SelfLocation, 5.0);

            if (Equal && ActorHasTag("PlacementMode") == false)
            {
                Destroy();
            }
        }
    }
}

void AUSInteractable::PlayWobble(FVector NewParam)
{
    FVector ActorLocation = GetActorLocation();
    FVector Direction = (NewParam - ActorLocation).GetSafeNormal(0.0001f);

    if (Mesh)
    {
        Mesh->SetVectorParameterValueOnMaterials(TEXT("Wobble Vector"), Direction);
    }

    if (WobbleTimeline)
    {
        WobbleTimeline->PlayFromStart();
    }
}

void AUSInteractable::EndWobble()
{
    if (Mesh)
    {
        Mesh->SetVectorParameterValueOnMaterials(TEXT("Wobble Vector"), FVector::ZeroVector);
    }

    if (WobbleTimeline)
    {
        WobbleTimeline->Stop();
    }
}

void AUSInteractable::HandleTimelineUpdate(float Value)
{
    if (Mesh)
    {
        Mesh->SetScalarParameterValueOnMaterials(TEXT("Wobble"), Value);
    }
}

void AUSInteractable::HandleTimelineFinished()
{
}

float AUSInteractable::Interact()
{
    return 0.0f;
}

void AUSInteractable::SetProgressionsState(float Progression)
{
    //if (RequireBuild == false)
    //    return;

    ProgressionState = Progression;
    //Tags.Add(FName("Build"));

    int32 MeshIndex = FMath::TruncToInt(ProgressionState);

    if (MeshList.IsValidIndex(MeshIndex))
    {
        UStaticMesh* NewMesh = MeshList[MeshIndex];

        if (Mesh)
        {
            Mesh->SetStaticMesh(NewMesh);
        }
    }
}

void AUSInteractable::SetEnableGroundBlend(bool bEnable)
{
    EnableGroundBlend = bEnable;
}

float AUSInteractable::GetProgressionState()
{
    return ProgressionState;
}
