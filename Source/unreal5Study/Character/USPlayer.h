// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USCharacterBase.h"
#include "InputActionValue.h"
#include "Character/Input/USInputContextData.h"
#include "Character/Camera/USCameraData.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "USPlayer.generated.h"



UENUM(BlueprintType)
enum class EInputKey : uint8
{
	None UMETA(DisplayName = "None"),
	Look UMETA(DisplayName = "Look"),
	Move UMETA(DisplayName = "Move"),
	Jump UMETA(DisplayName = "Jump"),
	CameraChange UMETA(DisplayName = "CameraChange"),
	ClickMove UMETA(DisplayName = "ClickMove"),
	MouseWheel UMETA(DisplayName = "MouseWheel"),
	MouseLClick UMETA(DisplayName = "MouseLClick"),
	MouseRClick UMETA(DisplayName = "MouseRClick"),
	LockOn UMETA(DisplayName = "LockOn"),
};

UENUM(BlueprintType)
enum class ECharacterInputState : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Grappling  UMETA(DisplayName = "Grappling"),
};

/**
 * 
 */
UCLASS()
class UNREAL5STUDY_API AUSPlayer : public AUSCharacterBase
{
	GENERATED_BODY()

public:
	AUSPlayer();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

public:
	// 특별히 인풋 매핑을 바꿀일이 있다면 여기서 변경 할 것
	void SetInputContextChange(class UInputMappingContext* MappingContext);
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ClickMove();

	virtual void Jump() override;

	void ClickInputClear();

	UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
	void K2_RotateRight(double Yaw, FRotator CharacterRotation);
	UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
	void K2_RotateLeft(double Yaw, FRotator CharacterRotation);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, Category = InputContext)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// 인풋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TMap<EInputKey, TObjectPtr<class UInputAction>> InputActionMap;

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UUserWidget> HUDWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Capture)
	TMap< EViewType, TObjectPtr<class USceneCaptureComponent2D>> SceneCapture;

public:
	UCameraComponent* GetFollowCamera();

public:
	void LockOnTarget();
	void UnlockTarget();
	void ToggleLockOn();
	void UpdateCameraLockOn(float DeltaTime);

private:
	AActor* CurrentTarget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Partners")
	TArray<TObjectPtr<class AUSPartner>> USPartnerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Partners")
	TSubclassOf<class AUSPartner> PartnerBPClass;

public:
	UFUNCTION(BlueprintCallable, Category = "Partners")
    void AddPartner();

public:
	virtual void PossessedBy(AController* NewController) override;

	void EquipWeaponCallBack(const struct FGameplayEventData* EventData);
	void EquipShieldCallBack(const struct FGameplayEventData* EventData);

	void OnMouseRClickTrigger();
	void OnMouseRClickComplete();
	void OnMouseLClickTrigger();
	void OnMouseLClickComplete();
	
	UFUNCTION()
	void OnOutOfHealth();

	void MoveSetting(ECharacterInputState CharacterState);

protected:
	UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UUSInventory> InventoryComponent;


protected:

	UPROPERTY(EditAnywhere)
    TObjectPtr<class AActor> BPCable;

	UPROPERTY(EditAnywhere)
	ECharacterInputState CharacterInputState = ECharacterInputState::None;

public:
	UFUNCTION()
	void SetCharacterInputState(ECharacterInputState InputState);
};
