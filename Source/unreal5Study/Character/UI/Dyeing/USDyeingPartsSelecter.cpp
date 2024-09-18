// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingPartsSelecter.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "DyeingGameInstance.h"
#include "../WidgetGameInstance.h"

void UUSDyeingPartsSelecter::NativeConstruct()
{
	Super::NativeConstruct();

    int32 Index = 0;
    ButtonList.Empty();
    // VerticalBox의 모든 자식 위젯을 순회
    for (UWidget* ChildWidget : VerticalBox->GetAllChildren())
    {
        // 해당 자식이 UButton인지 확인
        UButton* Button = Cast<UButton>(ChildWidget);
        if (Button)
        {
            SaveStyle = Button->GetStyle();
            // 버튼 리스트에 추가
            ButtonList.Add(Index++, Button);
            if(Button->OnPressed.IsAlreadyBound(this, &UUSDyeingPartsSelecter::OnTabButtonClicked) == false)
                Button->OnPressed.AddDynamic(this, &UUSDyeingPartsSelecter::OnTabButtonClicked);
        }
    }

    if (ButtonList.Num() > 0)
    {
        SelectButton(ButtonList[0]);
    }
}

void UUSDyeingPartsSelecter::OnTabButtonClicked()
{
    for (int32 i = 0; i < ButtonList.Num(); i++)
    {
        // 스타일을 변경할게 아니라 덮어쓸 이미지 한장 만들어야겠다...
        if (ButtonList[i]->IsPressed())
        {
            UE_LOG(LogTemp, Warning, TEXT("TabButton %d clicked"), i);

            SelectButton(ButtonList[i]);
        }
        else
        {
            ButtonList[i]->SetStyle(SaveStyle);
        }
    }
}

void UUSDyeingPartsSelecter::SelectButton(UButton* Button)
{
    FButtonStyle ButtonStyle = SaveStyle;
    FSlateBrush SelectedBrush;
    SelectedBrush.TintColor = FSlateColor(FLinearColor::Black);
    ButtonStyle.SetNormal(SelectedBrush);
    Button->SetStyle(ButtonStyle);

    //UWidgetMessage WidgetMessage;
    //SendMessage(EWidgetID::DyeingPalette, 0, &WidgetMessage);
}
