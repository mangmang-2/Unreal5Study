// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Actor/WorldPingActor.h"

// Sets default values
AWorldPingActor::AWorldPingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorldPingActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeTime);
}

// Called every frame
void AWorldPingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

