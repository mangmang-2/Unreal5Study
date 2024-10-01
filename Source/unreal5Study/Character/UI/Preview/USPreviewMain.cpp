// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Preview/USPreviewMain.h"
#include "USPreviewSlotGrid.h"
#include "../../USCharacterBase.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "Components/Button.h"

UUSPreviewMain::UUSPreviewMain(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUSPreviewMain::NativeConstruct()
{
	Super::NativeConstruct();
	if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance()))
	{
		ModularSubsystem->OnPreviewMainChange.AddDynamic(this, &UUSPreviewMain::OnPreviewChange);
	}
	if (Button_Apply)
	{
		Button_Apply->OnClicked.Clear();
		Button_Apply->OnClicked.AddDynamic(this, &UUSPreviewMain::OnApplyButtonClicked);
	}

	if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance()))
	{
		ModularSubsystem->OnPreviewClear.Broadcast();

		AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
		if (CharacterOwner)
		{
			const auto& ModularList = CharacterOwner->ModularCharacterComponent->GetModularList();
			for (int32 i = 0 ; i < ModularList.Num(); i++)
			{
				FModularCharacterRaw raw;
				raw.ModularCategory = static_cast<EItemCategory>(i);
				raw.ModularMesh = ModularList[i]->GetSkeletalMeshAsset();
				ModularSubsystem->OnPreviewChange.Broadcast(raw);

				OnPreviewChange(raw);
			}
		}
	}
}

void UUSPreviewMain::NativeDestruct()
{
	Super::NativeDestruct();

	if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance()))
	{
		ModularSubsystem->OnPreviewMainChange.Clear();
	}
}

void UUSPreviewMain::OnPreviewChange(FModularCharacterRaw ModularRaw)
{
	int8 Key = static_cast<int8>(ModularRaw.ModularCategory);
	if (!UsePreviewModular.Contains(Key))
	{
		// 키가 존재하지 않으면 데이터 삽입
		UsePreviewModular.Add(Key, ModularRaw);
	}
	else
	{
		UsePreviewModular[Key] = ModularRaw;
	}
}

void UUSPreviewMain::OnApplyButtonClicked()
{
	AUSCharacterBase* CharacterOwner = Cast<AUSCharacterBase>(GetOwningPlayerPawn());
	if (CharacterOwner == nullptr)
		return;

	CharacterOwner->ModularCharacterComponent->ClearAllParts();
	for (auto& Modular : UsePreviewModular)
	{
		CharacterOwner->ModularCharacterComponent->ChangeParts(static_cast<int8>(Modular.Key), Modular.Value.ModularMesh);
	}
}
