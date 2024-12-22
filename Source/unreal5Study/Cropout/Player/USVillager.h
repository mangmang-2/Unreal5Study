// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "USCropoutPawn.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "USVillager.generated.h"


UCLASS()
class UNREAL5STUDY_API AUSVillager : public AUSCropoutPawn, public IUSCharacterAIInterface
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


	virtual float GetAIPatrolRadius() { return 0.0f; };
	virtual float GetAIDetectRange() { return 0.0f; };
	virtual float GetAIAttackRange() { return 600.0f; };
	virtual float GetAITurnSpeed() { return 0.0f; };
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) {};
};
