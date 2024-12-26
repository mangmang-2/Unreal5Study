// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USCropoutPawn.h"
#include "USVillager.generated.h"


UCLASS()
class UNREAL5STUDY_API AUSVillager : public AUSCropoutPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUSVillager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual float PlayDeliverAnim_Implementation() override;
	virtual void PlayWorkAnim_Implementation(float Delay) override;
	virtual void ReturnToDefaultBT_Implementation() override;

	virtual void RemoveResource_Implementation(enum EResourceType& Resource, int32& Value)override;
	virtual void AddResource_Implementation(enum EResourceType Resource, int32 Value)override;

};
