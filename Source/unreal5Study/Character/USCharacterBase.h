// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "USCharacterBase.generated.h"

UCLASS()
class UNREAL5STUDY_API AUSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> EquipSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> EquipShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> UnequipSword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> UnequipShield;

public:
	FORCEINLINE class UStaticMeshComponent* GetWeaponMesh() { return EquipSword; }
	bool HitCheck(FVector StartPoint, FVector EndPoint, FHitResult& HitResult, bool DrawLine, float DrawLineTime, bool DebugMessage);
	bool CapsuleHitCheck(FVector CapsuleOrigin, float CapsuleRadius, float CapsuleHalfHeight, FHitResult& HitResult);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> ImpactMontage;


	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

public:
	//void NormalAttack();
	//bool WeaponAttackCheck(TSet<AActor*>& HitActors);
	//bool AttackCheck(TSet<AActor*>& HitActors);
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//void ComboActionBegin();
	//void ComboActionEnd();
	//void SetComboCheckTimer();
	//void ComboCheck();
	//int32 GetMaxCombo();

	//virtual void NotifyComboActionEnd();

public:
	float HalfAngle = 45.0;
	float DeltaAngle = 1.0;
	float ExternalRadius = 1500.0;

	UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UProceduralMeshComponent> ProceduralMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UUSClimbingComponent> ClimbingComponent;

	bool IsClimbing();
	bool IsClimbingMontage();
	bool IsClimbingFalling();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsCombatState: 1; 

	void ShowSword(bool bShow);
	void ShowShield(bool bShow);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TObjectPtr<class UAbilitySystemComponent> ASCComponent;

	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
};

