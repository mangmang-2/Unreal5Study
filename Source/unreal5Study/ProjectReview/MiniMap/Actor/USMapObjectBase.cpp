// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Actor/USMapObjectBase.h"

// Sets default values
AUSMapObjectBase::AUSMapObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUSMapObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSMapObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

