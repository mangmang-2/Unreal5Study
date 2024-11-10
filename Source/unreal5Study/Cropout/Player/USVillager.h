// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "USVillager.generated.h"

UCLASS()
class UNREAL5STUDY_API AUSVillager : public APawn
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


protected:
	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> Hat;

	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> Hair;

	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Tool;

	UPROPERTY(Category = Villager, EditAnywhere)
	TObjectPtr<class UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(Category = Villager, EditAnywhere)
	TObjectPtr<class UDecalComponent> Decal;
};
