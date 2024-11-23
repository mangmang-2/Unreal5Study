// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USInteractable.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AUSInteractable::AUSInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUSInteractable::BeginPlay()
{
	Super::BeginPlay();
	
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this; // 콜백 대상
    LatentInfo.ExecutionFunction = "AfterDelay"; // 지연 후 호출할 함수 이름
    LatentInfo.Linkage = 0; // Latent Action의 고유 ID
    LatentInfo.UUID = 1; // Unique ID

    // DelayUntilNextTick 호출
    UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
}
