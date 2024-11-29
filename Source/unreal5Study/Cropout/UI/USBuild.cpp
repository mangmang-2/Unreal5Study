// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/UI/USBuild.h"
#include "../Interactable/USInteractable.h"
#include "USBuildItem.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/USPlayerInterface.h"

void UUSBuild::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();

	UDataTable* DataTablePointer = LoadObject<UDataTable>(nullptr, TEXT("/Game/Study/Cropout/Spawn/DT_Buidables.DT_Buidables"));

	if (DataTablePointer != nullptr)
	{
		TArray<FUSResource*> AllRows;
		DataTablePointer->GetAllRows(nullptr, AllRows);

		for (auto& Row : AllRows)
		{
			AddItem(Row);
		}
	}

	OnActivated();
}

void UUSBuild::AddItem(struct FUSResource* Resource)
{
	if (Resource == nullptr)
		return;

	TSubclassOf<UUSBuildItem> BuildItemClass = LoadClass<UUSBuildItem>(nullptr, TEXT("/Game/Study/Cropout/UI/WBP_BuildItem.WBP_BuildItem_C"));
	if (BuildItemClass == nullptr)
		return;

	UUSBuildItem* BuildItem = CreateWidget<UUSBuildItem>(GetWorld(), BuildItemClass);
	if (BuildItem == nullptr)
		return;
	BuildItem->SetData(Resource);

	UHorizontalBoxSlot* NewSlot = ItemList->AddChildToHorizontalBox(BuildItem);
	if (NewSlot)
	{
		NewSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
		NewSlot->SetHorizontalAlignment(HAlign_Center);
		NewSlot->SetVerticalAlignment(VAlign_Bottom);
	}
}

void UUSBuild::OnActivated()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr)
		return;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetWidgetToFocus(TakeWidget());

	//PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;


	APawn* TargetActor = PlayerController->GetPawn();
	/*if (TargetActor)
	{
		TargetActor->SetActorTickEnabled(false);
		TargetActor->DisableInput(PlayerController);
	}*/

	IUSPlayerInterface* TargetPlayer = Cast<IUSPlayerInterface>(TargetActor);
	if (TargetPlayer == nullptr)
		return;
	TargetPlayer->SwitchBuildMode(true);

}
