// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USBuildHouse.h"
#include "../../GameMode/USCropoutGameMode.h"

void AUSBuildHouse::SpawnVillagers()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(GameMode);
		if (CropoutGameMode)
		{
			CropoutGameMode->SpawnVillager();
		}
	}
}

void AUSBuildHouse::ConstructComplete()
{
	Super::ConstructComplete();
	SpawnVillagers();
	Tags.Add(FName("Player"));
}
