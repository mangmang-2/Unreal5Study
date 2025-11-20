// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCMapGenerator.generated.h"

UCLASS()
class UNREAL5STUDY_API AMCMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMCMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	TMap<int32, TObjectPtr<class UMCBlockMesh>> InstMap;

	void MakeInstMesh();
};
