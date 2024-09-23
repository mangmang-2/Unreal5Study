#pragma once
#include "CoreMinimal.h"
#include "../WidgetGameInstance.h"
#include "../../../Data/ModularCharacterDataSubsystem.h"
#include "USDyeingData.generated.h"


UCLASS()
class UNREAL5STUDY_API UDyeingMessage : public UWidgetMessage
{
    GENERATED_BODY()
public:
    uint8 PartsType = 0;
    uint8 ColorParts = 0;
    FLinearColor Color;

    FModularCharacterRaw ModularData;
};