// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/USLevelTransfer.h"
#include "Components/BoxComponent.h"
#include "../Character/USPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "USLevelTransferSubsystem.h"

// Sets default values
AUSLevelTransfer::AUSLevelTransfer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransferBoard = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferBoard"));
	RootComponent = TransferBoard;
	TransferBoard->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

AUSLevelTransfer::~AUSLevelTransfer()
{
}


// Called when the game starts or when spawned
void AUSLevelTransfer::BeginPlay()
{
	Super::BeginPlay();
}

void AUSLevelTransfer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AUSPlayer* pPlayer = Cast<AUSPlayer>(OtherActor);
	if (pPlayer)
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(pPlayer->GetController());
		if (PlayerController)
		{
			UUSLevelTransferSubsystem* LevelTransferSubsystem = ULocalPlayer::GetSubsystem<UUSLevelTransferSubsystem>(PlayerController->GetLocalPlayer());
			if (LevelTransferSubsystem)
			{

				LevelTransferSubsystem->SetTransferSequenceFrameTime(TransferSequenceFrameTime, 1.0f);
			}
		}
	}
}

// Called every frame
void AUSLevelTransfer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

