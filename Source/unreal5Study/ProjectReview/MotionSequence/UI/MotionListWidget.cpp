// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MotionSequence/UI/MotionListWidget.h"
#include "MotionMainWidget.h"
#include "../MotionSeqenceSubsystem.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBoxSlot.h"

void UMotionListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UMotionSeqenceSubsystem* SeqenceSubsystem = UGameInstance::GetSubsystem<UMotionSeqenceSubsystem>(GetGameInstance()))
	{
        const TArray<FMotionItem>& MotionNames = SeqenceSubsystem->GetMotionList();
        if (ScrollBox && RowWidgetClass)
        {
            ScrollBox->ClearChildren();

            for (const FMotionItem& Label : MotionNames) // 버튼에 표시할 텍스트 목록
            {
                UUserWidget* Row = CreateWidget<UUserWidget>(GetWorld(), RowWidgetClass);
                // 버튼 가져오기
                UButton* Btn = Cast<UButton>(Row->GetWidgetFromName("Button"));
                UTextBlock* Text = Cast<UTextBlock>(Row->GetWidgetFromName("TextBlock"));
                if(Btn == nullptr)
                    continue;

                if (Text == nullptr)
                    continue;

                UScrollBoxSlot* TemplateSlot = Cast<UScrollBoxSlot>(Btn->Slot);
                if (TemplateSlot == nullptr)
                    continue;

                Text->SetText(FText::FromString(Label.MotionName));
                Btn->AddChild(Text);

                Btn->OnClicked.Clear();
                Btn->OnClicked.AddDynamic(this, &UMotionListWidget::OnClickMotionButton);

                MotionButtons.Add(Btn);

                // ScrollBox에 추가하면서 슬롯 반환받기
                UScrollBoxSlot* NewSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(Btn));

                // 슬롯 옵션 복사
                if (TemplateSlot && NewSlot)
                {
                    NewSlot->SetPadding(TemplateSlot->Padding);
                    NewSlot->SetHorizontalAlignment(TemplateSlot->HorizontalAlignment);
                    NewSlot->SetVerticalAlignment(TemplateSlot->VerticalAlignment);
                }
            }
        }
	}

    Button_Apply->OnClicked.AddDynamic(this, &UMotionListWidget::OnClickApplyButton);
}

void UMotionListWidget::OnMainWidgetMoved(FVector2D Pos, FVector2D Size)
{
	FVector2D Target = Pos + FVector2D(Size.X, 0.f);
	SetPositionInViewport(Target, true);
}

void UMotionListWidget::OnClickMotionButton()
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

void UMotionListWidget::ClickMotionButton(int32 Index)
{
    for (UButton* Btn : MotionButtons)
    {
        Btn->SetStyle(NormalStyle);
    }

    UButton* ClickedBtn = Cast<UButton>(MotionButtons[Index]);
    if (ClickedBtn)
    {
        SelectIndex = Index;
        ClickedBtn->SetStyle(SelectedStyle);
    }
}

void UMotionListWidget::OnClickApplyButton()
{
    if(SelectIndex >= 0)
        OnMotinListSelect.Broadcast(SelectIndex);
}
