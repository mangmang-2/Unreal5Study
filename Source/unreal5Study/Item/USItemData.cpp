// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/USItemData.h"

FLinearColor FUSItemData::GetOriginColor(uint8 ColorParts) const
{
    if (ColorParts < 0 || ColorParts >= ItemOriginPartsColor.Num())
        return FLinearColor();
    return ItemOriginPartsColor[ColorParts];
}

FLinearColor FUSItemData::GetChangeColor(uint8 ColorParts) const
{
    if (ColorParts < 0 || ColorParts >= ItemOriginPartsColor.Num())
        return FLinearColor();
    return ItemChangePartsColor[ColorParts];
}

void FUSItemData::SetOriginColor(uint8 ColorParts, FLinearColor Color)
{
    if (ColorParts < 0 || ColorParts >= ItemOriginPartsColor.Num())
        return;
    ItemOriginPartsColor[ColorParts] = Color;
}

void FUSItemData::SetChangeColor(uint8 ColorParts, FLinearColor Color)
{
    if (ColorParts < 0 || ColorParts >= ItemOriginPartsColor.Num())
        return;
    ItemChangePartsColor[ColorParts] = Color;
}
