// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Island/USSpawner.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "USSpawner.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../GameMode/USCropoutGameMode.h"

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
	AsyncLoadClasses();

    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
    if (GameMode)
    {
        AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(GameMode);
        if (CropoutGameMode)
        {
            if (!CropoutGameMode->OnLoadCompleted.IsAlreadyBound(this, &AUSSpawner::SpawnRandom))
            {
                CropoutGameMode->OnLoadCompleted.AddDynamic(this, &AUSSpawner::SpawnRandom);
            }
        }
    }
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
    /* 이런 방법으로 사용해야함
    * TSoftClassPtr<AActor> ClassRef = TSoftClassPtr<AActor>(FSoftObjectPath("/Game/Blueprints/BP_Tree.BP_Tree"));

if (ClassRef.IsValid())
{
    // 자산이 이미 로드되어 있다면 UClass*로 변환
    UClass* LoadedClass = ClassRef.Get();

    // UClass*가 ATree의 서브클래스인지 확인하고 캐스팅
    if (LoadedClass->IsChildOf(ATree::StaticClass()))
    {
        // 안전하게 ATree 클래스로 변환
        TSubclassOf<ATree> TreeClass = LoadedClass;

        // TreeClass로 작업 진행
        ATree* SpawnedTree = GetWorld()->SpawnActor<ATree>(TreeClass);
    }
}
else
{
    // 비동기 로드
    ClassRef.LoadAsync(FStreamableDelegate::CreateLambda([=]()
    {
        UClass* LoadedClass = ClassRef.Get();
        if (LoadedClass && LoadedClass->IsChildOf(ATree::StaticClass()))
        {
            // 안전하게 ATree 클래스로 변환
            TSubclassOf<ATree> TreeClass = LoadedClass;

            // TreeClass로 작업 진행
            ATree* SpawnedTree = GetWorld()->SpawnActor<ATree>(TreeClass);
        }
    }));
}
    */
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
    //int32 BiomeCount = 3;
    //int32 MaxSpawn = 3;
    //for (int32 IndexCounter = 0; IndexCounter < BiomeCount; ++IndexCounter)
    //{
    //    UInstancedStaticMeshComponent* InstancedMeshComponent = NewObject<UInstancedStaticMeshComponent>(this);
    //    if (InstancedMeshComponent)
    //    {
    //        InstancedMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    //        InstancedMeshComponent->RegisterComponent();

    //        UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Path/To/YourMesh.YourMesh"));
    //        if (NewMesh)
    //        {
    //            InstancedMeshComponent->SetStaticMesh(NewMesh);
    //        }

    //        for (int32 SpawnIndex = 0; SpawnIndex < MaxSpawn; ++SpawnIndex)
    //        {
    //            FTransform InstanceTransform;
    //            InstanceTransform.SetLocation(FVector::ZeroVector);
    //            //InstanceTransform.SetScale3D(FVector(BiomeScale)); 

    //            InstancedMeshComponent->AddInstance(InstanceTransform);
    //        }
    //    }
    //}

    //FActorSpawnParameters SpawnParams;
    //SpawnParams.Owner = this;
    //if (ClassRef)
    //{
    //    for (int32 i = 0; i < BiomeCount; ++i)
    //    {
    //        FVector SpawnLocation = FVector(i * 100.0f, 0.0f, 0.0f); // Adjust spawn location as needed
    //        FRotator SpawnRotation = FRotator::ZeroRotator;

    //        AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ClassRef, SpawnLocation, SpawnRotation, SpawnParams);
    //        if (SpawnedActor)
    //        {

    //        }
    //    }
    //}
}

FVector AUSSpawner::GetRandomPoint()
{
    UWorld * World = GetWorld();
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);

    if (NavSystem == nullptr)
        return FVector::ZeroVector;
    FVector Origin = { 0.0f, 0.0f, 0.0f };
    float Radius = 1000.0;
    FNavLocation RandomLocation;
    bool bSuccess = NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLocation);
    NavSystem->GetRandomPoint(RandomLocation);
    return bSuccess ? RandomLocation.Location : RandomLocation;
}

void AUSSpawner::PickPointsAroundBiomePoints(class UInstancedStaticMeshComponent* Mesh, FVector BiomeCenter, float Radius, int32 BiomeCount, int32 MaxSpawn)
{
    UWorld* World = GetWorld();

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
    if (NavSystem == nullptr) 
        return;

    FRandomStream RandomStream;
    int32 LoopCount = RandomStream.RandRange(0, MaxSpawn);

    for (int32 i = 0; i < LoopCount; i++)
    {
        FNavLocation RandomLocation;
        bool bSuccess = NavSystem->GetRandomReachablePointInRadius(BiomeCenter, Radius, RandomLocation);

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
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    FRandomStream RandomStream;
    for (int32 i = 0; i < SpawnData.BiomeCount; i++)
    {
        FVector Origin = FVector(0.0f, 0.0f, 0.0f);
        FNavLocation Pos;
        if (NavSys->GetRandomPointInNavigableRadius(Origin, 10000.0f, Pos))
        {
            int32 LoopCount = UKismetMathLibrary::RandomIntegerInRangeFromStream(0, SpawnData.SpawnPerBiome, RandomStream);
            for (int32 Index = 0; Index < LoopCount; Index++)
            {
                FNavLocation SpawnPos;
                if (NavSys->GetRandomPointInNavigableRadius(Pos, SpawnData.BiomeScale, SpawnPos))
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
