// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/USCropoutGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "../Cropout/Island/USSpawner.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include <Kismet/KismetMathLibrary.h>
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void AUSCropoutGameMode::BeginPlay()
{
	Super::BeginPlay();
    GetSpawnRef();
}

void AUSCropoutGameMode::GetSpawnRef()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, FoundActors);

    if (FoundActors.Num() > 0)
    {
        SpawnRef = Cast<AUSSpawner>(FoundActors[0]);
    }
}

void AUSCropoutGameMode::IslandGencomplete()
{
    BeginAsyncSpawning(); 
}

void AUSCropoutGameMode::BeginAsyncSpawning()
{
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(TownHallRef.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AUSCropoutGameMode::OnAsyncLoadComplete));
}

void AUSCropoutGameMode::OnAsyncLoadComplete()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnMarkerClass, FoundActors);

    if (FoundActors.Num() > 0)
    {
        SpawnRef = Cast<AUSSpawner>(FoundActors[0]);
    }

    OnTownHallClassLoaded();
    SpawnVillager();
    SpawnInteractables();
}

void AUSCropoutGameMode::OnTownHallClassLoaded()
{
    if (TownHallRef.IsValid() == false)
        return;

    // 로드된 클래스 확인 및 캐스팅
    UClass* TownHallClass = TownHallRef.Get();
    if (TownHallClass == nullptr)
        return;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnMarkerClass, FoundActors);

    if (FoundActors.Num() > 0)
    {
        // 랜덤으로 스폰 마커 선택
        AActor* RandomMarker = FoundActors[FMath::RandRange(0, FoundActors.Num() - 1)];

        // 선택된 마커 위치 가져오기
        FVector SpawnLocation = RandomMarker->GetActorLocation();
        FRotator SpawnRotation = RandomMarker->GetActorRotation();

        FVector NewPosition = GetSteppedPosition(SpawnLocation, 200.0);

        // 타운 홀 액터 스폰
        FActorSpawnParameters SpawnParams;
        AActor* SpawnedTownHall = GetWorld()->SpawnActor<AActor>(TownHallClass, NewPosition, SpawnRotation, SpawnParams);

        // 스폰된 타운 홀을 변수에 저장
        if (SpawnedTownHall)
        {
            TownHall = SpawnedTownHall;
        }

    }
}

FVector AUSCropoutGameMode::GetSteppedPosition(const FVector& Position, float StepSize)
{
    // Position 벡터의 X와 Y를 StepSize에 맞춰 반올림
    float SteppedX = FMath::RoundToFloat(Position.X / StepSize) * StepSize;
    float SteppedY = FMath::RoundToFloat(Position.Y / StepSize) * StepSize;

    // 새로운 벡터 반환 (Z는 0으로 설정)
    return FVector(SteppedX, SteppedY, 0.0f);
}

FVector AUSCropoutGameMode::GetRandomPointInBounds()
{
    FVector Origin, BoxExtent;
    TownHall->GetActorBounds(false, Origin, BoxExtent); 

    FVector RandomUnitVector = UKismetMathLibrary::RandomUnitVector();

    FVector CalcVector = (RandomUnitVector * FMath::Min(BoxExtent.X, BoxExtent.Y) * 2.0f) + Origin;
    CalcVector.Z = 0;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    FNavLocation NavLocation;
    if (NavSystem && NavSystem->GetRandomReachablePointInRadius(CalcVector, 500.0f, NavLocation))
    {
        return NavLocation.Location;
    }

    return FVector::ZeroVector;
}

void AUSCropoutGameMode::SpawnVillager()
{
    if (VillagerRef == nullptr)
        return;

    for (int32 i = 0; i < 3; i++)
    {
        FVector SpawnLocation = GetRandomPointInBounds();
        FRotator SpawnRotation = FRotator::ZeroRotator;

        SpawnLocation.Z += 92.561032;

        APawn* SpawnedVillager = UAIBlueprintHelperLibrary::SpawnAIFromClass(
            GetWorld(),
            VillagerRef,
            nullptr,            // AI에 적용할 Behavior Tree
            SpawnLocation,
            SpawnRotation,
            true                // true로 설정하면 충돌할 경우 실패하지 않고 근처에 스폰 시도
        );

        if(SpawnedVillager)
            VillagerCount++;
    }
    
    OnUpdateVillagers.Broadcast(VillagerCount);
}

void AUSCropoutGameMode::SpawnInteractables()
{
    ReadyToSpawn();
}

void AUSCropoutGameMode::ReadyToSpawn()
{
    int32 IndexCounter = 0;
    // 타이머 설정 (0.5초마다 네비게이션 빌드 상태 확인)
    GetWorldTimerManager().SetTimer(NavCheckHandle, this, &AUSCropoutGameMode::CheckNavigationBuild, 0.5f, true, -0.5f);
}

void AUSCropoutGameMode::CheckNavigationBuild()
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    if (NavSys && !NavSys->IsNavigationBuildingLocked())
    {
        // 네비게이션 빌드가 완료된 경우 타이머 정지
        GetWorldTimerManager().ClearTimer(NavCheckHandle);

        // 이후 로직 수행 (예: 스폰 작업)
        OnLoadCompleted.Broadcast();
    }
}
