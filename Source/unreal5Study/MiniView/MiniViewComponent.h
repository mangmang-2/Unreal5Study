// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MiniViewComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UMiniViewComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMiniViewComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class USceneCaptureComponent2D> sceneCapture;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TObjectPtr<class UTextureRenderTarget2D> renderTarget;

public:
	void SetComponent(class USceneComponent* InParent);

};
