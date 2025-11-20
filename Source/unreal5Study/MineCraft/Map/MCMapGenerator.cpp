// Fill out your copyright notice in the Description page of Project Settings.


#include "MineCraft/Map/MCMapGenerator.h"
#include "MCMapGenerator.h"

// Sets default values
AMCMapGenerator::AMCMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMCMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	MakeInstMesh();
}

// Called every frame
void AMCMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMCMapGenerator::MakeInstMesh()
{
}

