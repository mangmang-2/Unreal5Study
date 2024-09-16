#pragma once
#include "CoreMinimal.h"
#include "USWidgetID.generated.h"

UENUM()
enum class EWidgetID : uint32
{
	None,
	PreviewMain			UMETA(DisplayName = "PreviewMain"),
	PreviewSlotGrid		UMETA(DisplayName = "PreviewSlotGrid"),
	PreviewSlot			UMETA(DisplayName = "PreviewSlot"),
};