// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/USUserWidget.h"

void UUSUserWidget::SetOwningActor(AActor* InOwner)
{
	OwningActor = InOwner;
}

void UUSUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
    
    // 커서를 보이게 설정하고, 게임과 UI 모두 입력 가능하게 설정
    if (GetWorld())
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            PC->bShowMouseCursor = true;

            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            InputMode.SetWidgetToFocus(TakeWidget()); // 현재 위젯에 포커스를 맞춤

            PC->SetInputMode(InputMode);
        }
    }
}

void UUSUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

    // 커서를 숨기게 설정
    if (GetWorld())
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            PC->bShowMouseCursor = false;
            PC->SetInputMode(FInputModeGameOnly());
        }
    }
}

FReply UUSUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    return FReply::Handled();
}

FReply UUSUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    return FReply::Handled();
}
