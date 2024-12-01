// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Cropout/Interactable/USResourceInterface.h"
#include "USCropoutGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnUpdateVillagers, int32, VillagerCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadCompleted);

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSCropoutGameMode : public AGameModeBase, public IUSResourceInterface
{
	GENERATED_BODY()

public:	
	virtual void BeginPlay() override;

	void GetSpawnRef();

	UFUNCTION()
	void IslandGencomplete();

	void BeginAsyncSpawning();
	UFUNCTION()
	void OnAsyncLoadComplete();
	void OnTownHallClassLoaded();
	
	FVector GetSteppedPosition(const FVector& Position, float StepSize);

	FVector GetRandomPointInBounds();
	void SpawnVillager();

	void SpawnInteractables();

	UFUNCTION()
	void ReadyToSpawn();

	UFUNCTION()
	void CheckNavigationBuild();

	void AddResource_Implementation(enum EResourceType Resource, int32 Value);
	virtual void RemoveTargetResource(enum EResourceType Resource, int32 Value);
	virtual int32 GetCurrentResources(enum EResourceType Resource);
	void RemoveCurrentUILayer();

	void EndGame();

	void SendUIResourceValue();
	TMap<EResourceType, int32> GetResources();
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnMarkerClass;
	
	UPROPERTY()
	TObjectPtr<class AUSSpawner> SpawnRef;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> TownHallRef;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> TownHall;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> VillagerRef;

	int32 VillagerCount = 0;
	FTimerHandle NavCheckHandle;

	UPROPERTY(EditAnywhere)
	TMap<EResourceType, int32> Resources;
public:
	FOnUpdateVillagers OnUpdateVillagers;
	FOnLoadCompleted OnLoadCompleted;

};
