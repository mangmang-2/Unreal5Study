// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Island/USSpawner.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "USSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../GameMode/USCropoutGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Interactable/USInteractable.h"

// Sets default values
AUSSpawner::AUSSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUSSpawner::BeginPlay()
{
	Super::BeginPlay();
	//AsyncLoadClasses();
}

void AUSSpawner::AsyncLoadClasses()
{
    ClassRefIndex = 0;
    bAsyncComplete = false; 

    if (SpawnTypes.Num() > 0)
    {
        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
        for (int32 Index = 0; Index < SpawnTypes.Num(); Index++)
        {
            TSoftClassPtr<AActor> ClassRef = SpawnTypes[Index].ClassREf;

            if (ClassRef.IsValid())
            {
                // 클래스가 이미 로드되었을 때
                OnAsyncLoadComplete();
            }
            else
            {
                // 비동기 로드
                Streamable.RequestAsyncLoad(ClassRef.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AUSSpawner::OnAsyncLoadComplete));
            }
            //ClassRef->GenerateFunctionList()
        }
    }
}

void AUSSpawner::OnAsyncLoadComplete()
{
    ClassRefIndex++;

    // 모든 클래스가 로드되었는지 확인
    if (ClassRefIndex >= SpawnTypes.Num())
    {
        bAsyncComplete = true;
        // 모든 클래스가 로드되었으면 이제 스폰을 생성함
        SpawnInstances();
    }

    ReadyToSpawn();
}

void AUSSpawner::SpawnInstances()
{
    SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
    FVector CenterPos = GetRandomPoint();
	for (int32 IndexCounter = 0; IndexCounter < SpawnInstance.Num(); ++IndexCounter)
	{
        UInstancedStaticMeshComponent* InstancedMeshComponent = NewObject<UInstancedStaticMeshComponent>(this);
        InstancedMeshComponent->RegisterComponent();
        InstancedMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        //InstancedMeshComponent = Cast<UInstancedStaticMeshComponent>(AddComponent(FName("DynamicMesh"), false, FTransform(), nullptr));

        InstancedMeshComponent->SetStaticMesh(SpawnInstance[IndexCounter].ClassREf);
        PickPointsAroundBiomePoints(InstancedMeshComponent, CenterPos, SpawnInstance[IndexCounter].BiomeScale, SpawnInstance[IndexCounter].BiomeCount, SpawnInstance[IndexCounter].SpawnPerBiome);
        
	}
}

FVector AUSSpawner::GetRandomPoint()
{
    if(NavData == nullptr)
        return FVector::ZeroVector;
    UWorld * World = GetWorld();
    FVector Origin = { 0.0f, 0.0f, 0.0f };
    float Radius = 1000.0;
    FNavLocation RandomLocation;
    bool bSuccess = NavData->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation);
    //RandomLocation = NavData->GetRandomPoint();
    return bSuccess ? RandomLocation.Location : RandomLocation;
}

void AUSSpawner::PickPointsAroundBiomePoints(class UInstancedStaticMeshComponent* Mesh, FVector BiomeCenter, float Radius, int32 BiomeCount, int32 MaxSpawn)
{
    if (NavData == nullptr)
        return;

    UWorld* World = GetWorld();

    FRandomStream RandomStream;
    int32 LoopCount = RandomStream.RandRange(MaxSpawn / 2, MaxSpawn);

    for (int32 i = 0; i < LoopCount; i++)
    {
        FNavLocation RandomLocation;
        bool bSuccess = NavData->GetRandomReachablePointInRadius(BiomeCenter, Radius, RandomLocation);

        if (bSuccess)
        {
            FTransform InstanceTransform;
            InstanceTransform.SetLocation(RandomLocation.Location);
            InstanceTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));
            InstanceTransform.SetScale3D(FVector(1.f, 1.f, 1.f));
            Mesh->AddInstance(InstanceTransform);
        }
    }
}

FTransform AUSSpawner::GenerateRandomTransform(FVector Pos, FVector SpawnPos, float Radius)
{
    float VectorLength = FVector::Dist(Pos, SpawnPos);
    float ScaleValue = FMath::Lerp(0.8f, 1.5f, VectorLength / Radius);

    FTransform NewTransform;
    NewTransform.SetLocation(FVector(SpawnPos.X, SpawnPos.Y, 0.0));
    NewTransform.SetRotation(FRotator(0.0f, 0.0f, 0.0f).Quaternion());
    NewTransform.SetScale3D(FVector(ScaleValue)); 

    return NewTransform;
}

void AUSSpawner::SpawnRandom()
{
    for (int32 Index = 0; Index < SpawnTypes.Num(); Index++)
    {
        TSoftClassPtr<AActor> ClassRef = SpawnTypes[Index].ClassREf;
        if (ClassRef.IsValid())
        {
            SpawnAssets(ClassRef.Get(), SpawnTypes[Index]);
        }
    }
}

void AUSSpawner::SpawnAssets(TSubclassOf<AActor> ClassToSpawn, FSTSpawnData SpawnData)
{
    if (NavData == nullptr)
        return;

    FRandomStream RandomStream;
    for (int32 i = 0; i < SpawnData.BiomeCount; i++)
    {
        FVector Origin = FVector(0.0f, 0.0f, 0.0f);
        FNavLocation Pos;
        if (NavData->GetRandomPointInNavigableRadius(Origin, 10000.0f, Pos))
        {
            int32 LoopCount = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, SpawnData.SpawnPerBiome, RandomStream);
            for (int32 Index = 0; Index < LoopCount; Index++)
            {
                FNavLocation SpawnPos;
                if (NavData->GetRandomPointInNavigableRadius(Pos, SpawnData.BiomeScale, SpawnPos))
                {
                    SpawnActor(ClassToSpawn, SpawnData, SpawnPos);
                }
            }
        }
    }
}

void AUSSpawner::SpawnActor(TSubclassOf<AActor> ClassToSpawn, FSTSpawnData SpawnData, FNavLocation SpawnPos)
{
    FVector Location = SteppedPosition(SpawnPos.Location);

    float RandomYaw = UKismetMathLibrary::RandomFloatInRange(0, SpawnData.RandomRotationRange);
    FRotator Rotation = FRotator(0.0f, RandomYaw, 0.0f);

    float RandomScale = UKismetMathLibrary::RandomFloatInRange(1.0f, SpawnData.ScaleRange + 1);
    FVector Scale = FVector(RandomScale, RandomScale, RandomScale);

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(Location);
    SpawnTransform.SetRotation(FQuat(Rotation));
    SpawnTransform.SetScale3D(Scale);

    // 스폰 파라미터 설정
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    // 액터 스폰
    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);
    if (SpawnedActor)
    {
    }
}

FVector AUSSpawner::SteppedPosition(FVector NewParam)
{
    float SteppedX = FMath::RoundToFloat(NewParam.X / 200.0f) * 200.0f;
    float SteppedY = FMath::RoundToFloat(NewParam.Y / 200.0f) * 200.0f;
    float SteppedZ = 0.0f;

    return FVector(SteppedX, SteppedY, SteppedZ);
}

void AUSSpawner::ReadyToSpawn()
{
    int32 IndexCounter = 0;
    // 타이머 설정 (0.5초마다 네비게이션 빌드 상태 확인)
    GetWorldTimerManager().SetTimer(NavCheckHandle, this, &AUSSpawner::CheckNavigationBuild, 0.5f, true, -0.5f);
}

void AUSSpawner::CheckNavigationBuild()
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    if (NavSys && !NavSys->IsNavigationBuildingLocked())
    {
        // 네비게이션 빌드가 완료된 경우 타이머 정지
        GetWorldTimerManager().ClearTimer(NavCheckHandle);

        // 이후 로직 수행 (예: 스폰 작업)
        SpawnRandom();

        AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(UGameplayStatics::GetGameMode(this));
        if (CropoutGameMode)
        {
            CropoutGameMode->UpdateAllInteractables();
        }
    }
}

