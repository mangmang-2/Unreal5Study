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

	void InitLeaderPose();

public:
	void RandomChange();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ModularList, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class USkeletalMeshComponent>> ModularList;		
};
