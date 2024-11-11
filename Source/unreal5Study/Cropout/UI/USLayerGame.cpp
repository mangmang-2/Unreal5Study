// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USLayerGame.h"
#include "../../GameMode/USCropoutGameMode.h"

void UUSLayerGame::NativeConstruct()
{
	Super::NativeConstruct();

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(GameMode);
		if (CropoutGameMode)
		{
			if (!CropoutGameMode->OnUpdateVillagers.IsAlreadyBound(this, &UUSLayerGame::UpdateVillagers))
			{
				CropoutGameMode->OnUpdateVillagers.AddDynamic(this, &UUSLayerGame::UpdateVillagers);
			}
		}
	}
}

void UUSLayerGame::UpdateVillagers(int32 VillagerCount)
{
}
