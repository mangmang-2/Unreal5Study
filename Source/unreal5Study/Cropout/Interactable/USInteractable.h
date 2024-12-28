// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USResourceInterface.h"
#include "USInteractable.generated.h"


USTRUCT(BlueprintType)
struct FUSCostIcon : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType CostType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> UIIcon;
};

USTRUCT(BlueprintType)
struct FUSResource : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class AUSInteractable> TargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> UIIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EResourceType, int32> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor TabCol;
};


UCLASS(Blueprintable, BlueprintType)
class UNREAL5STUDY_API AUSInteractable : public AActor, public IUSResourceInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void DoDestroy();

public:
	UFUNCTION(BlueprintCallable)
	void PlacementMode();

protected:
	UFUNCTION()
    void AfterDelay();

	void WriteToRenderTarget();
	void TransformToTexture(FVector2D Size, FVector2D& ReturnValue, FVector2D& ReturnValue2);
	void CheckForOverlappingActors();

	UFUNCTION(BlueprintCallable)
	void PlayWobble(FVector NewParam);

	UFUNCTION(BlueprintCallable)
	void EndWobble();

    UFUNCTION()
    void HandleTimelineUpdate(float Value);

    UFUNCTION()
    void HandleTimelineFinished();

	UFUNCTION(BlueprintCallable)

	virtual float Interact();

public:
	virtual void SetProgressionsState(float Progression);
	void SetEnableGroundBlend(bool bEnable);
	float GetProgressionState();
protected:
	UPROPERTY(EditAnywhere)
	bool EnableGroundBlend = false;

	float OutlineDraw = 1.5f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> RTDraw;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> RenderMaterial;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	float ProgressionState = 0;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UStaticMesh>> MeshList;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> WobbleTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCurveFloat> WobbleCurve;

	UPROPERTY(EditAnywhere)
    float CollectionTime = 3.0f;
	
	UPROPERTY(EditAnywhere)
    bool RequireBuild = false;
};
