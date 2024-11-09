// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "USIslandGenerator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaskComplete);
/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSIslandGenerator : public ADynamicMeshActor
{
	GENERATED_BODY()
	
public:
	AUSIslandGenerator();
	
	virtual void BeginPlay() override;

protected:
	void SpawnCone();
	void SpawnMarker();
	void AppendBox();
	void MeshSlidify();
	void SmoothMesh();
	void Tesselation();

	void MeshPlaneCut();
	void ProjectUvs();
	void ReleaseCompute();
	void SetIslandColor();
private:
	int32 Islands = 20; // 만들어질 지형 개수
	float MaxSpawnDistance = 9776.351562;
	float IslandSizeX = 800.0;
	float IslandSizeY = 5000.0;

	FOnTaskComplete OnTaskComplete;
	TArray<FVector> SpawnPoints;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnMarkerClass;
};
