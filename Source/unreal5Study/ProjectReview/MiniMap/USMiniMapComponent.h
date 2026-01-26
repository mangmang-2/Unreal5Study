// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "USMiniMapComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UUSMiniMapComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSMiniMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY()
	TObjectPtr<class USceneCaptureComponent2D> SceneCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTextureRenderTarget2D> RenderTarget;

public:
	UPROPERTY(EditAnywhere, Category="MiniMap")
	float Height = 10000.0f;

	UPROPERTY(EditAnywhere, Category="MiniMap")
	float OrthoWidth = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniMap")
	bool bRotateWithOwnerYaw = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MiniMap")
	float CaptureInterval = 0.2f;

	class UTextureRenderTarget2D* GetRenderTarget() const { return RenderTarget; }
		
private:
	float Accum = 0.f;
	void UpdateCaptureTransform();
	void DoCapture();
};
