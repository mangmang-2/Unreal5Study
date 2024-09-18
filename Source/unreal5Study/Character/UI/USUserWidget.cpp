// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/USUserWidget.h"
#include "WidgetGameInstance.h"


void UUSUserWidget::SetOwningActor(AActor* InOwner)
{
	OwningActor = InOwner;
}

void UUSUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (OwningActor)
    {
        if (UWidgetGameInstance* WidgetGameInstance = UGameInstance::GetSubsystem<UWidgetGameInstance>(OwningActor->GetGameInstance()))
        {
            // 존재하면 다시 넣지않음
            if (WidgetGameInstance->WidgetMessageMap[static_cast<uint32>(WidgetID)].IsAlreadyBound(this, &ThisClass::DelegateMessage) == false)
            {
                WidgetGameInstance->WidgetMessageMap[static_cast<uint32>(WidgetID)].AddDynamic(this, &ThisClass::DelegateMessage);
            }
        }
    }
    
    // 커서를 보이게 설정하고, 게임과 UI 모두 입력 가능하게 설정
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetWidgetToFocus(TakeWidget()); // 현재 위젯에 포커스를 맞춤

        PC->SetInputMode(InputMode);
    }
}

void UUSUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

    // 커서를 숨기게 설정
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void UUSUserWidget::DelegateMessage(int32 MessageType, UWidgetMessage* WidgetMessage)
{
    ResponseMessage(MessageType, WidgetMessage);
}

void UUSUserWidget::ResponseMessage(int32 MessageType, UWidgetMessage* WidgetMessage)
{
}

void UUSUserWidget::SendMessage(EWidgetID SendWidgetID, int32 MessageType, UWidgetMessage* WidgetMessage)
{
    if (OwningActor)
    {
        if (UWidgetGameInstance* WidgetGameInstance = UGameInstance::GetSubsystem<UWidgetGameInstance>(OwningActor->GetGameInstance()))
        {
            if (WidgetGameInstance->WidgetMessageMap.Contains(static_cast<uint32>(SendWidgetID)))
            {
                WidgetGameInstance->WidgetMessageMap[static_cast<uint32>(SendWidgetID)].Broadcast(MessageType, WidgetMessage);
            }
        }
    }
}
