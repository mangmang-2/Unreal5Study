// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UI/USUserWidget.h"
#include "USDyeingPalette.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSDyeingPalette : public UUSUserWidget
{
	GENERATED_BODY()

public:
	UUSDyeingPalette(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UTexture2D* CreateGradationTexture(UObject* Outer, int32 Width, int32 Height);
	FColor GetPixelColor(int32 X, int32 Y);

	// 테스트를 위해서 임시로 사용하는 함수
	void ChangeModulPartsColor(FColor SelectColor);

public:

	UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> Palette;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr < class UMaterialInterface> ColorMaterial;

	TArray<FColor> Pixels;
	int32 TextureSize = 500;
};
