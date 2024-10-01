// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ModularCharacter/USModularCharacterComponent.h"
#include "GameFramework/Character.h"
#include "../../Data/ModularCharacterDataSubsystem.h"

// Sets default values for this component's properties
UUSModularCharacterComponent::UUSModularCharacterComponent()
{
	for (uint8 i = 0; i < static_cast<uint8>(EItemCategory::MAX); i++)
	{
		FString DisplayName = StaticEnum<EItemCategory>()->GetDisplayNameTextByIndex(i).ToString();

		ModularList.Add(CreateDefaultSubobject<USkeletalMeshComponent>(*DisplayName));
	}
}

// Called when the game starts
void UUSModularCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	InitLeaderPose();
	//RandomChange();
}

void UUSModularCharacterComponent::InitLeaderPose()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	auto Mesh = owner->GetMesh();
	if (Mesh == nullptr)
		return;

	for (auto Modular : ModularList)
	{
		Modular->SetLeaderPoseComponent(Mesh);
		Modular->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void UUSModularCharacterComponent::RandomChange()
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;

	UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(owner->GetGameInstance());
	if (ModularSubsystem)
	{
		for (uint8 i = 0; i < static_cast<uint8>(EItemCategory::MAX); i++)
		{
			ModularList[i]->SetSkeletalMesh(nullptr);
			EItemCategory eCategory = static_cast<EItemCategory>(i);
			if (FModularCharacterRaw ModularData; FMath::RandRange(0, 5) > 0 && ModularSubsystem->GetRandomModular(eCategory, ModularData))
			{
				ModularList[i]->SetSkeletalMesh(ModularData.ModularMesh);
			}
		}
	}
}

void UUSModularCharacterComponent::ClearAllParts()
{
	for (auto& Modular : ModularList)
	{
		Modular->SetSkeletalMesh(nullptr);
	}
}

void UUSModularCharacterComponent::ChangeParts(FModularCharacterRaw ModularRaw)
{
	uint8 eCategory = static_cast<uint8>(ModularRaw.ModularCategory);
	if (ModularList.IsValidIndex(eCategory) == false)
		return;

	ChangeParts(eCategory, ModularRaw.ModularMesh);
}

void UUSModularCharacterComponent::ChangeParts(FUSItemData ItemData)
{
	uint8 eCategory = static_cast<uint8>(ItemData.ItemCategory);
	if (ModularList.IsValidIndex(eCategory) == false)
		return;

	ChangeParts(eCategory, ItemData.ItemMesh);
	
	for (uint8 i = 0; i < static_cast<uint8>(EItemColorParts::MAX); i++)
	{
		ChangePartsColor(eCategory, i, ItemData.GetChangeColor(i));
	}
}

void UUSModularCharacterComponent::ChangeParts(uint8 eCategory, USkeletalMesh* ModularMesh)
{
	ModularList[eCategory]->SetSkeletalMesh(ModularMesh);
	ModularList[eCategory]->SetMaterial(0, nullptr);
}

void UUSModularCharacterComponent::ChangePartsColor(uint8 PartsType, uint8 ColorParts, FLinearColor Color)
{
	if (ModularList.IsValidIndex(PartsType) == false)
		return;

	UMaterialInstanceDynamic* DynMaterial = ModularList[PartsType]->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		FString ColorPartsName = StaticEnum<EItemColorParts>()->GetDisplayNameTextByIndex(ColorParts).ToString();

		DynMaterial->SetVectorParameterValue(FName(*ColorPartsName), Color);
	}
}

void UUSModularCharacterComponent::InitPartsColor(uint8 PartsType, uint8 ColorParts)
{
	ModularList[PartsType]->SetMaterial(0, nullptr);
}

const TArray<TObjectPtr<USkeletalMeshComponent>>& UUSModularCharacterComponent::GetModularList()
{
	return ModularList;
}