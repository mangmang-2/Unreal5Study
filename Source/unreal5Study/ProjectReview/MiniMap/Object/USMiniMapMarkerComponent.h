// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectData.h"
#include "USMiniMapMarkerComponent.generated.h"


// 하나의 객체당 마커 컴포넌트 하나 부여
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UUSMiniMapMarkerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSMiniMapMarkerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitMarkerIconClass();

	FVector GetWorldLocation() const;

	bool IsVisible() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<class UUSMiniMapMarkerWidget> MarkerIconClass;

	UPROPERTY(EditAnywhere)
    EObjectType MarkerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
};
