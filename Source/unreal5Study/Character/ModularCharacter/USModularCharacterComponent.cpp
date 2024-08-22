// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ModularCharacter/USModularCharacterComponent.h"
#include "GameFramework/Character.h"
#include "../../Data/ModularCharacterDataSubsystem.h"

// Sets default values for this component's properties
UUSModularCharacterComponent::UUSModularCharacterComponent()
{
	for (uint8 i = 0; i < static_cast<uint8>(EModularCharacterType::MAX); i++)
	{
		FString DisplayName = StaticEnum<EModularCharacterType>()->GetDisplayNameTextByIndex(i).ToString();

		ModularList.Add(CreateDefaultSubobject<USkeletalMeshComponent>(*DisplayName));
	}
}

// Called when the game starts
void UUSModularCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	InitLeaderPose();
	RandomChange();
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
		for (uint8 i = 0; i < static_cast<uint8>(EModularCharacterType::MAX); i++)
		{
			ModularList[i]->SetSkeletalMesh(nullptr);
			EModularCharacterType eCategory = static_cast<EModularCharacterType>(i);
			if (FModularCharacterRaw ModularData; FMath::RandRange(0, 5) > 0 && ModularSubsystem->GetRandomModular(eCategory, ModularData))
			{
				ModularList[i]->SetSkeletalMesh(ModularData.ModularMesh);
			}
		}
	}
}
