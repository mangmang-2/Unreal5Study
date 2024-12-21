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
#include "../Lyra/GameFramework/GameplayMessageSubsystem.h"
#include "../Cropout/UI/USResourceUIItem.h"
#include "NavigationPath.h"

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
    OnMonsterHallClassLoaded();
    for (int32 i = 0; i < 3; i++)
    {
        SpawnVillager();
    }

    SendUIResourceValue();
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

void AUSCropoutGameMode::OnMonsterHallClassLoaded()
{
    //MonsterHall

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnMarkerClass, FoundActors);
    if (FoundActors.Num() <= 0)
        return;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSystem == nullptr)
        return;

    FVector TownHallLocation = TownHall->GetActorLocation();
    float MaxDistance = 0.0f;
    AActor* FarthestActor = nullptr;
    for (auto& Actor : FoundActors)
    {
        FVector ActorLocation = Actor->GetActorLocation();

        UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(GetWorld(), TownHallLocation, ActorLocation);
        if (NavPath && NavPath->IsValid())
        {
            float PathLength = NavPath->GetPathLength();
            if (PathLength > MaxDistance)
            {
                MaxDistance = PathLength;
                FarthestActor = Actor;
            }
        }
    }

    if (FarthestActor)
    {
        FVector SpawnLocation = FarthestActor->GetActorLocation();
        FRotator SpawnRotation = FarthestActor->GetActorRotation();

        FActorSpawnParameters SpawnParams;
        AActor* MonsterTownHall = GetWorld()->SpawnActor<AActor>(MonsterHallClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (MonsterTownHall)
        {
            MonsterHall = MonsterTownHall;
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
    
    OnUpdateVillagers.Broadcast(VillagerCount);
}


int32 AUSCropoutGameMode::GetCurrentResources(enum EResourceType Resource)
{
    return Resources[Resource];
}

void AUSCropoutGameMode::RemoveCurrentUILayer()
{
}

void AUSCropoutGameMode::EndGame()
{
}

void AUSCropoutGameMode::SendUIResourceValue()
{
    FCropoutResourceValueMessageData Message;
    Message.Verb = TAG_Cropout_UI_Message;
    Message.Resources = Resources;

    UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSystem.BroadcastMessage(Message.Verb, Message);
}

TMap<EResourceType, int32> AUSCropoutGameMode::GetResources()
{
    return Resources;
}

void AUSCropoutGameMode::RemoveTargetResource(EResourceType Resource, int32 Value)
{
    if (Resources.Find(Resource) == nullptr)
        return;

    Resources[Resource] -= Value;

    if (Resources[EResourceType::Food] <= 0)
        EndGame();

    SendUIResourceValue();
}

void AUSCropoutGameMode::AddResource_Implementation(EResourceType Resource, int32 Value)
{
    if (Resources.Find(Resource) == nullptr)
        return;

    Resources[Resource] += Value;

    SendUIResourceValue();
}
