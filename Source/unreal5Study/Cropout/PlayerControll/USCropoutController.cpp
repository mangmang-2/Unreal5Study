// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/PlayerControll/USCropoutController.h"

void AUSCropoutController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스 커서가 뷰포트에 고정되지 않도록 설정
   // SetInputMode(InputMode); // 입력 모드 설정
    //bShowMouseCursor = true; // 마우스 커서 표시
}
