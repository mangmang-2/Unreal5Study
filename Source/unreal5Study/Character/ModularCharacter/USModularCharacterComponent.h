// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "USModularCharacterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL5STUDY_API UUSModularCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUSModularCharacterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Helmet2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Gloves;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Pants;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> ArmArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> PantsArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> ShoulderArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Modular, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Torso;
		
};
