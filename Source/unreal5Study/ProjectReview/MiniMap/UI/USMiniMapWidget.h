// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "USMiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API UUSMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> MinimapImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> PlayerArrow = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArrowYawOffset = 0.0f;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY()
	TObjectPtr<class UUSMiniMapComponent> MiniMapComponent;

	bool bDragging = false;
	FVector2D LastMousePos;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> IconLayer;

	UPROPERTY()
	TMap<TObjectPtr<class UUSMiniMapMarkerComponent>, TObjectPtr<class UUSMiniMapMarkerWidget>> MarkerIconMap;

	UPROPERTY()
	FVector2D MapSize;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWorldPingActor> PingActorClass;

public:
	void DrawMarkers();
	bool IsInMiniMap(const FVector& WorldPos) const;
	FVector2D ConvertWorldToMiniMap(const FVector& WorldPos) const;
	void CreateIcon(class UUSMiniMapMarkerComponent* Marker);

	UFUNCTION()
	void HandleMarkerClicked(class UUSMiniMapMarkerComponent* Marker);

	void SpawnWorldPing(FVector WorldPos);
};
