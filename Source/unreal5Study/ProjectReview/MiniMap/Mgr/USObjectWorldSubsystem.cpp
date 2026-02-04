// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Mgr/USObjectWorldSubsystem.h"
#include "USObjectWorldSubsystem.h"
#include "USObjectGameInstanceSubsystem.h"
#include "../Object/USMiniMapMarkerComponent.h"
#include "../Actor/USMapObjectBase.h"

void UUSObjectWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	SpawnMapObjects();
}

void UUSObjectWorldSubsystem::SpawnMapObjects()
{
	UUSObjectGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSObjectGameInstanceSubsystem>(GetWorld()->GetGameInstance());
	if (Subsystem == nullptr)
		return;

	const auto& ObjectDatas = Subsystem->GetObjects();

    for (const auto& data : ObjectDatas)
    {
        if (data.TargetClass.IsNull())
            continue;

        if (IsAlreadySpawned(data.ObjectPos))
            continue;

        UClass* SpawnClass = data.TargetClass.LoadSynchronous();
        if (SpawnClass == nullptr)
            continue;

        AActor* Spawned = GetWorld()->SpawnActor<AActor>(SpawnClass,
                                                        data.ObjectPos,
                                                        FRotator::ZeroRotator);

        if (Spawned == nullptr) 
            continue;

        SpawnedActors.Add(Spawned);

        if (data.bShowOnMiniMap)
        {
            auto Marker = Spawned->FindComponentByClass<UUSMiniMapMarkerComponent>();

            if (Marker == nullptr)
            {
                Marker = NewObject<UUSMiniMapMarkerComponent>(Spawned);
                Marker->RegisterComponent();
            }

            Marker->MarkerType = data.MarkerType;
            Marker->bVisible = true;
            Marker->Description = FText::FromString(*data.Name);
            Marker->InitMarkerIconClass();
        }
    }
}

bool UUSObjectWorldSubsystem::IsAlreadySpawned(const FVector& Pos) const
{
    for (auto Actor : SpawnedActors)
    {
        if (IsValid(Actor) == false) 
            continue;

        if (FVector::Dist(Actor->GetActorLocation(), Pos) < 10.f)
            return true;
    }
    return false;
}
