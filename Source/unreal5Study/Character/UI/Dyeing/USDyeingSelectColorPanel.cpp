// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingSelectColorPanel.h"
#include "USDyeingData.h"
#include "USDyeingColorDetail.h"

UUSDyeingSelectColorPanel::UUSDyeingSelectColorPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    WidgetID = EWidgetID::DyeingSelectColorPanel;
}


void UUSDyeingSelectColorPanel::ResponseMessage(int32 MessageType, UWidgetMessage* WidgetMessage)
{
	UDyeingMessage* DyeingMessage = Cast<UDyeingMessage>(WidgetMessage);
	switch (MessageType)
    {
    case 0: // 원본
        SetColor(WBP_DyeingColorDetail_Origin, DyeingMessage->Color);
        break;
    case 1: // 선택한 색상
        SetColor(WBP_DyeingColorDetail_Select, DyeingMessage->Color);
        break;
    default:
        break;
    }
}

void UUSDyeingSelectColorPanel::SetColor(UUSDyeingColorDetail* Detail, FLinearColor Color)
{
    if (Detail == nullptr)
        return;
    
    Detail->SetColor(Color);

}
