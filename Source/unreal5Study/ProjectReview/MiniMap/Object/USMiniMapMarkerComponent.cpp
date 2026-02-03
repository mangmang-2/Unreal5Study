// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Object/USMiniMapMarkerComponent.h"
#include "USMiniMapMarkerComponent.h"
#include "../Mgr/USMiniMapGameInstanceSubsystem.h"

// Sets default values for this component's properties
UUSMiniMapMarkerComponent::UUSMiniMapMarkerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUSMiniMapMarkerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UUSMiniMapGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSMiniMapGameInstanceSubsystem>(GetWorld()->GetGameInstance()))
	{
		Subsystem->RegisterMarker(this);
	}
	
}

void UUSMiniMapMarkerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UUSMiniMapGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSMiniMapGameInstanceSubsystem>(GetWorld()->GetGameInstance()))
	{
		Subsystem->UnregisterMarker(this);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UUSMiniMapMarkerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUSMiniMapMarkerComponent::ApplyIconTexture()
{
	if (UUSMiniMapGameInstanceSubsystem* Subsystem = UGameInstance::GetSubsystem<UUSMiniMapGameInstanceSubsystem>(GetWorld()->GetGameInstance()))
	{
		MarkerTexture = Subsystem->GetIcon(MarkerType);
	}
}

FVector UUSMiniMapMarkerComponent::GetWorldLocation() const
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner) == false)
		return FVector::ZeroVector;

	return GetOwner()->GetActorLocation();
}

bool UUSMiniMapMarkerComponent::IsVisible() const
{
	return bVisible;
}

