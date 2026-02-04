// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USMiniMapMarkerWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMarkerClicked, class UUSMiniMapMarkerComponent*, Marker);
/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSMiniMapMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleClick();

public:
    void Init(class UUSMiniMapMarkerComponent* Marker);

    UFUNCTION()
    void HandleHovered();

    UFUNCTION()
    void HandleUnhovered();

public:
	UPROPERTY(BlueprintAssignable)
    FOnMarkerClicked OnMarkerClicked;

protected:
    UPROPERTY()
    TObjectPtr<class UUSMiniMapMarkerComponent> MarkerComponent;

    UPROPERTY(meta=(BindWidget))
    TObjectPtr<class UButton> ClickButton;

    UPROPERTY(meta=(BindWidget))
    TObjectPtr<class UImage> IconImage;
};
