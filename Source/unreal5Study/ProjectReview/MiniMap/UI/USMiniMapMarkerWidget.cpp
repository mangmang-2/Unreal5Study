// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/UI/USMiniMapMarkerWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "../Object/USMiniMapMarkerComponent.h"

void UUSMiniMapMarkerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ClickButton)
    {
        ClickButton->OnClicked.AddDynamic(this, &UUSMiniMapMarkerWidget::HandleClick);
        ClickButton->OnHovered.AddDynamic(this, &UUSMiniMapMarkerWidget::HandleHovered);
        ClickButton->OnUnhovered.AddDynamic(this, &UUSMiniMapMarkerWidget::HandleUnhovered);
    }
}

void UUSMiniMapMarkerWidget::HandleClick()
{
    OnMarkerClicked.Broadcast(MarkerComponent);
}

void UUSMiniMapMarkerWidget::Init(UUSMiniMapMarkerComponent* Marker)
{
    MarkerComponent = Marker;
}

void UUSMiniMapMarkerWidget::HandleHovered()
{
    if(MarkerComponent == nullptr)
        return;

    SetToolTipText(MarkerComponent->Description);
}

void UUSMiniMapMarkerWidget::HandleUnhovered()
{
    SetToolTip(nullptr);
}
