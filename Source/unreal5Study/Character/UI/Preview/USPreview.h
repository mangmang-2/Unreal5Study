// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../USCharacterBase.h"
#include "USPreview.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUICreated);

UCLASS()
class UNREAL5STUDY_API AUSPreview : public AUSCharacterBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPreviewChange(struct FModularCharacterRaw ModularRaw);

	UFUNCTION()
	void OnPreviewChangeByMesh(uint8 eCategory, class USkeletalMesh* ModularMesh);

	UFUNCTION()
	void OnPreviewClear();

};
