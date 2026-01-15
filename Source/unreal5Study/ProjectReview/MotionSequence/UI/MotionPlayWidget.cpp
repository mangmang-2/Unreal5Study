// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MotionSequence/UI/MotionPlayWidget.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "MotionListWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MotionMainWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../MotionSeqenceSubsystem.h"

void UMotionPlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (UniformGridPanel)
	{
		const int32 ChildCount = UniformGridPanel->GetChildrenCount();

		for (int32 i = 0; i < ChildCount; ++i)
		{
			if (UWidget* Child = UniformGridPanel->GetChildAt(i))
			{
				if (UButton* Btn = Cast<UButton>(Child))
				{
					Btn->OnClicked.Clear();
					Btn->OnClicked.AddDynamic(this, &UMotionPlayWidget::OnClickMotionButton);
					MotionButtons.Add(Btn);
				}
			}
		}
	}

}

void UMotionPlayWidget::OnClickMotionButton()
{
	for (int32 i = 0; i < MotionButtons.Num(); ++i)
	{
		if (MotionButtons[i]->HasKeyboardFocus())
		{
			ClickMotionButton(i);
			break;
		}
	}

}

void UMotionPlayWidget::ClickMotionButton(int32 Index)
{
	SelectIndex = Index;
	ToggleListView();
}

void UMotionPlayWidget::ToggleListView()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	if (MotionListWidgetClass == nullptr)
		return;

	// 켜져있으면 끔
	if (MotionListWidget && MotionListWidget->IsInViewport())
	{
		MotionListWidget->RemoveFromParent();
		MotionListWidget = nullptr;
		return;
	}

	if (MotionListWidget == nullptr)
	{
		MotionListWidget = CreateWidget<UMotionListWidget>(World, MotionListWidgetClass);
	}

	if (MotionListWidget != nullptr)
	{
		MotionListWidget->AddToViewport();

		// 생성 할때 오른쪽 옆으로 초기 설정
		if(ParentMainWidget == nullptr)
			return;
		ParentMainWidget->OnMainWidgetMoved.AddDynamic(MotionListWidget, &UMotionListWidget::OnMainWidgetMoved);
		ParentMainWidget->RefreshPostion();

		//MotionListWidget->OnMotinListSelect.AddDynamic(MotionListWidget, &UMotionPlayWidget::OnClickApplyButton);
	}


}

void UMotionPlayWidget::OnClickApplyButton(int32 Index)
{
	/*if (UMotionSeqenceSubsystem* SeqenceSubsystem = UGameInstance::GetSubsystem<UMotionSeqenceSubsystem>(GetGameInstance()))
	{
		const FMotionItem& MotionItem = SeqenceSubsystem->GetMotionByID(Index);
	}*/
}
