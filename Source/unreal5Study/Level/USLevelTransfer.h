// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USLevelTransfer.generated.h"

UCLASS()
class UNREAL5STUDY_API AUSLevelTransfer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSLevelTransfer();

	virtual ~AUSLevelTransfer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = LevelTransfer)
	FString TransferLevelName;

	UPROPERTY(EditAnywhere, Category = LevelTransfer)
	int32 TransferSequenceFrameTime = 0;

	UPROPERTY(EditAnywhere, Category = LevelTransfer)
	TObjectPtr<class UBoxComponent> TransferBoard;

};
