// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSTSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> ClassREf;

	UPROPERTY(EditAnywhere)
	float BiomeScale;

	UPROPERTY(EditAnywhere)
	int32 BiomeCount;

	UPROPERTY(EditAnywhere)
	int32 SpawnPerBiome;

	UPROPERTY(EditAnywhere)
	float RandomRotationRange;

	UPROPERTY(EditAnywhere)
	float ScaleRange;
};

USTRUCT(BlueprintType)
struct FSTSpawnInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> ClassREf;

	UPROPERTY(EditAnywhere)
	float BiomeScale;

	UPROPERTY(EditAnywhere)
	int32 BiomeCount;

	UPROPERTY(EditAnywhere)
	int32 SpawnPerBiome;
};
UCLASS()
class UNREAL5STUDY_API AUSSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void AsyncLoadClasses();
	void OnAsyncLoadComplete();

	void SpawnInstances();
	FVector GetRandomPoint();
	void PickPointsAroundBiomePoints(class UInstancedStaticMeshComponent* Mesh, FVector BiomeCenter, float Radius, int32 BiomeCount, int32 MaxSpawn);
	FTransform GenerateRandomTransform(FVector Pos, FVector SpawnPos, float Radius);

	UFUNCTION()
	void SpawnRandom();
	void SpawnAssets(TSubclassOf<class AActor> ClassToSpawn, struct FSTSpawnData SpawnData);
	void SpawnActor(TSubclassOf<AActor> ClassToSpawn, struct FSTSpawnData SpawnData, struct FNavLocation SpawnPos);
	FVector SteppedPosition(FVector NewParam);
protected:
	UPROPERTY(EditAnywhere)
    TArray<FSTSpawnData> SpawnTypes;		// 실제 사용하는 자원

	UPROPERTY(EditAnywhere)
    TArray<FSTSpawnInstance> SpawnInstance; // 조경

    UPROPERTY()
    bool bAsyncComplete = false;  // 비동기 완료 플래그

    int32 ClassRefIndex = 0;  // 클래스 참조 인덱스

};
