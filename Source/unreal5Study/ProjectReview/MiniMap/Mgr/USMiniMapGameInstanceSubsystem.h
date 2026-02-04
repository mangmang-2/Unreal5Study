// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Object/ObjectData.h"
#include "USMiniMapGameInstanceSubsystem.generated.h"


USTRUCT()
struct FMarkerIconDef : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    EObjectType Type;

    UPROPERTY(EditAnywhere)
	TSoftClassPtr<class UUSMiniMapMarkerWidget> IconClass;
};

USTRUCT()
struct FMiniMapMarkerEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class UUSMiniMapMarkerComponent> MarkerComp = nullptr;
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSMiniMapGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void LoadIconTable();

	void RegisterMarker(class UUSMiniMapMarkerComponent* Marker);
	void UnregisterMarker(class UUSMiniMapMarkerComponent* Marker);

	const TArray<FMiniMapMarkerEntry>& GetMarkers() const;
	TSoftClassPtr<class UUSMiniMapMarkerWidget> GetIconClass(EObjectType Type);

private:
	UPROPERTY()
	TMap<EObjectType, TSoftClassPtr<class UUSMiniMapMarkerWidget>> IconMapData;

	UPROPERTY()
	TArray<FMiniMapMarkerEntry> ActiveMarkers;


};
