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
	LockOn UMETA(DisplayName = "LockOn"),
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
	void SetInputContextChange(class UInputMappingContext* InputMappingContext);
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CameraChange();
	void ClickMove();

	virtual void Jump() override;

	void ClickInputClear();

	virtual void SetViewData(const class UUSCameraData* CameraData);
	EViewType GetNextViewType(EViewType CurrentView);
	
	class USpringArmComponent* GetSpringArmComponent(EViewType ViewType) { return CemeraSprigArm[ViewType]; }
	class USceneCaptureComponent2D* GetSceneCaptureComponent(EViewType ViewType) { return SceneCapture[ViewType]; }

	void SetupCemeraSprigArm();
	void SetCameraSprigArm(EViewType ViewType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
	void K2_RotateRight(double Yaw, FRotator CharacterRotation);
	UFUNCTION(BlueprintImplementableEvent, Category = "Blueprint")
	void K2_RotateLeft(double Yaw, FRotator CharacterRotation);

protected:
	TMap<EViewType, TObjectPtr<class USpringArmComponent>> CemeraSprigArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 1인칭, 3인칭, 탑뷰 카메라 데이터......인데 input도 있어서 이 이름이면 안될것 같다...
	UPROPERTY(EditAnywhere, Category = CameraTypeMap, Meta = (AllowPrivateAccess = "true"))
	TMap<EViewType, class UUSCameraData*> CameraTypeMap;

	EViewType CurrentViewType = EViewType::FirstPerson;

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


	void ComboAttack();

};
