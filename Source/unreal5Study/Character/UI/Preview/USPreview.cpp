// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Preview/USPreview.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "../../ModularCharacter/USModularCharacterComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AUSPreview::AUSPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUSPreview::BeginPlay()
{
	Super::BeginPlay();

	if (UModularCharacterDataSubsystem* ModularSubsystem = UGameInstance::GetSubsystem<UModularCharacterDataSubsystem>(GetGameInstance()))
	{
		ModularSubsystem->OnPreviewChange.AddDynamic(this, &AUSPreview::OnPreviewChange);
		ModularSubsystem->OnPreviewClear.AddDynamic(this, &AUSPreview::OnPreviewClear);
		
	}
}

// Called every frame
void AUSPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUSPreview::OnPreviewChange(struct FModularCharacterRaw ModularRaw)
{
	if (ModularCharacterComponent)
	{
		ModularCharacterComponent->ChangeParts(ModularRaw);
	}
}

void AUSPreview::OnPreviewChangeByMesh(uint8 eCategory, class USkeletalMesh* ModularMesh)
{
	if (ModularCharacterComponent)
	{
		ModularCharacterComponent->ChangeParts(eCategory, ModularMesh);
	}
}

void AUSPreview::OnPreviewClear()
{
	if (ModularCharacterComponent)
	{
		ModularCharacterComponent->ClearAllParts();
	}
}

