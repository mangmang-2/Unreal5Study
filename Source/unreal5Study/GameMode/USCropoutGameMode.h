// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "USCropoutGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSCropoutGameMode : public AGameModeBase
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
};
