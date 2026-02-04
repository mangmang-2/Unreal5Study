// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Mgr/USMiniMapGameInstanceSubsystem.h"

void UUSMiniMapGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadIconTable();
}

void UUSMiniMapGameInstanceSubsystem::LoadIconTable()
{
	UDataTable* DataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/ProjectReview/MiniMap/Data/DT_MarkerData.DT_MarkerData"));

	if (DataTable == nullptr)
	{
		return;
	}

	TArray<FMarkerIconDef*> AllRows;
	DataTable->GetAllRows(nullptr, AllRows);

	for (auto& Row : AllRows)
	{
		IconMapData.Add(Row->Type, Row->IconClass);
	}
}

void UUSMiniMapGameInstanceSubsystem::RegisterMarker(UUSMiniMapMarkerComponent* Marker)
{
	if(Marker == nullptr)
		return;

	FMiniMapMarkerEntry Entry;
	Entry.MarkerComp = Marker;

	ActiveMarkers.Add(Entry);
}

void UUSMiniMapGameInstanceSubsystem::UnregisterMarker(UUSMiniMapMarkerComponent* Marker)
{
	if (Marker == nullptr)
		return;

	ActiveMarkers.RemoveAll(
		[Marker](const FMiniMapMarkerEntry& Entry)
		{
			return Entry.MarkerComp == Marker;
		});
}

const TArray<FMiniMapMarkerEntry>& UUSMiniMapGameInstanceSubsystem::GetMarkers() const
{
	return ActiveMarkers;
}

TSoftClassPtr<class UUSMiniMapMarkerWidget> UUSMiniMapGameInstanceSubsystem::GetIconClass(EObjectType Type)
{
	if (const auto Found = IconMapData.Find(Type))
	{
		return *Found;
	}

	return nullptr;
}
