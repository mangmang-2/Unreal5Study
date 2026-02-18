// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayerController.h"
#include "AIController.h"

AUSPlayerController::AUSPlayerController()
{
	//bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AUSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
