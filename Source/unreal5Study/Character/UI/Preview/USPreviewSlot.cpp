// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Preview/USPreviewSlot.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"

void UUSPreviewSlot::SetData(FModularCharacterRaw ModularRaw)
{
	ModularCharacterRaw = ModularRaw;
	// 아이콘
	ModularPartsIcon->SetBrushFromTexture(ModularRaw.ModularIcon);
	// 텍스트
	EditableTextBox_PartsName->SetText(FText::FromString(ModularRaw.ModularName));
}

void UUSPreviewSlot::OnModularPartsIconButtonClicked()
{
	if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance()))
	{
		ModularSubsystem->OnPreviewChange.Broadcast(ModularCharacterRaw);
		ModularSubsystem->OnPreviewMainChange.Broadcast(ModularCharacterRaw);
	}
}
