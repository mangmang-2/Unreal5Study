// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "USVillagerInterface.h"
#include "../Interactable/USResourceInterface.h"
#include "../../Interface/USCharacterAIInterface.h"
#include "USCropoutPawn.generated.h"

USTRUCT(BlueprintType)
struct FSTJob : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBehaviorTree> BehaviourTree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> WorkAnim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMesh> Hat;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMesh> Tool;
};


UCLASS()
class UNREAL5STUDY_API AUSCropoutPawn : public APawn, public IUSVillagerInterface, public IUSResourceInterface, public IUSCharacterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUSCropoutPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Action(class AActor* VillagerAction);
	virtual void ChangeJob(FName NewJob);
	void UpdateAllPawn();
	void ResetJobState();
	void StopJob();
	void BehaviorTreeLoaded(class UBehaviorTree* BehaviorTree);
	void LoadAndStoreAccessories(struct FSTJob* Job);



	void PlayVillagerAnim(class UAnimMontage* Montage, float Length);
	void OnMontageComplete();

	UFUNCTION()
	void OnHealthChange();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	TObjectPtr<class AActor> TargetRef;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> ActiveBehavior;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY()
	TObjectPtr<class UUSCropoutStat> AttributeSet;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<class UUSCropoutWidgetComponent> HPBarWidgetComponent;


protected:
	UPROPERTY(Category= Villager, EditAnywhere)
	TObjectPtr<class UStaticMesh> TargetTool;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> WorkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> PutDownAnim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    EResourceType ResourcesHeld;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMesh> CratMesh;

	UPROPERTY(EditAnywhere)
	float AICollisionRange = 200.0f;

public:
	virtual float GetAIPatrolRadius() { return 0.0f; };
	virtual float GetAIDetectRange() { return 0.0f; };
	virtual float GetAIAttackRange() { return 100.0f; };
	virtual float GetAITurnSpeed() { return 0.0f; };
	virtual float GetAICollisionRange() { return AICollisionRange; };
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) {};
};
