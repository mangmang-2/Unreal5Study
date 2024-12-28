 // Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USPause.h"
#include "Kismet/GameplayStatics.h"
#include "../../GameMode/USCropoutGameMode.h"

void UUSPause::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController == nullptr)
		return;

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UUSPause::NativeDestruct()
{
	Super::NativeDestruct();

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UUSPause::Resume()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);	
}

void UUSPause::Restart()
{
	AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(GetWorld()->GetAuthGameMode());
	if (CropoutGameMode)
	{
		CropoutGameMode->ClearGame();
	}

	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UUSPause::MainMenu()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
