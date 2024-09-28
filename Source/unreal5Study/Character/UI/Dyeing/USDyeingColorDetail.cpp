// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingColorDetail.h"
#include "USDyeingColorDetail.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUSDyeingColorDetail::InitText(FText InputColor)
{
	Color->SetText(InputColor);
}

void UUSDyeingColorDetail::SetColor(FLinearColor InputColor)
{
    FButtonStyle ButtonStyle;
    FSlateBrush SelectedBrush;
    SelectedBrush.TintColor = FSlateColor(InputColor);
    ButtonStyle.SetNormal(SelectedBrush);
    ButtonStyle.SetHovered(SelectedBrush);
    Button->SetStyle(ButtonStyle);
}

