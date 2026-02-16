// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/USCameraData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "../MiniView/MiniViewComponent.h"
#include "Components/CapsuleComponent.h"
#include "Target/USTargetableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "USPartner.h"
#include "Movement/USClimbingComponent.h"
#include "USPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Ability/Tag/USGameplayTag.h"
#include "Ability/Attribute/USChracterAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../Item/USInventory.h"
#include "Movement/USParkourComponent.h"
#include "USPlayer.h"
#include "USCharacterBase.h"
#include "Movement/GrapplingHookComponent.h"
#include "../ProjectReview/MiniMap/Object/USMiniMapComponent.h"
#include "../ProjectReview/MiniMap/Object/USMiniMapMarkerComponent.h"

AUSPlayer::AUSPlayer()
{

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetRef(TEXT("/Game/Study/Character/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}

	for (uint32 ViewType = (uint8)EViewType::None + 1; ViewType < (uint8)EViewType::Max; ++ViewType)
	{
		// 이름에 ViewType 값을 포함시켜 각 SceneCapture 컴포넌트에 고유한 이름을 생성합니다.
		FName ComponentName = *FString::Printf(TEXT("MiniViewSceneCapture_%d"), ViewType);
		SceneCapture.Add(static_cast<EViewType>(ViewType), CreateDefaultSubobject<USceneCaptureComponent2D>(ComponentName));
	}

	static ConstructorHelpers::FClassFinder<AUSPartner> PartnerBPClassRef(TEXT("/Script/Engine.Blueprint'/Game/Study/Character/BP_Partner.BP_Partner_C'"));
	if (PartnerBPClassRef.Succeeded())
	{
		PartnerBPClass = PartnerBPClassRef.Class;
	}

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Sword_and_Shield/Demo/Mannequin_UE4/Meshes/Shield.Shield'"));
	//if (BodyMeshRef.Object)
	//{
	//	UnequipShield->SetStaticMesh(BodyMeshRef.Object);
	//}
	InventoryComponent = CreateDefaultSubobject<UUSInventory>(TEXT("Inventory"));

	MiniMapComp = CreateDefaultSubobject<UUSMiniMapComponent>(TEXT("MiniMapComp"));
	MiniMapComp->SetupAttachment(RootComponent);

	MiniMapMarkerComponent = CreateDefaultSubobject<UUSMiniMapMarkerComponent>(TEXT("MiniMapMarkerComponent"));
	MiniMapMarkerComponent->MarkerType = EObjectType::Player;
	MiniMapMarkerComponent->bVisible = true;
	MiniMapMarkerComponent->Description = FText::FromString(TEXT("Player"));
}


void AUSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		HUDWidget->AddToViewport();
	}

	if(ClimbingComponent)
		ClimbingComponent->SetClimb(true);

	if(MiniMapMarkerComponent)
		MiniMapMarkerComponent->InitMarkerIconClass();
}

void AUSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (InputActionMap.Contains(EInputKey::Move))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Move], ETriggerEvent::Triggered, this, &ThisClass::Move);
	}

	if (InputActionMap.Contains(EInputKey::Look))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Look], ETriggerEvent::Triggered, this, &ThisClass::Look);
	}

	//
	//if (InputActionMap.Contains(EInputKey::CameraChange))
	//{
	//	EnhancedInputComponent->BindAction(InputActionMap[EInputKey::CameraChange], ETriggerEvent::Triggered, this, &ThisClass::CameraChange);
	//}

	if (InputActionMap.Contains(EInputKey::ClickMove))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::ClickMove], ETriggerEvent::Triggered, this, &ThisClass::ClickMove);
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::ClickMove], ETriggerEvent::Completed, this, &ThisClass::ClickMove);
	}

	if (InputActionMap.Contains(EInputKey::Jump))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Jump], ETriggerEvent::Triggered, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Jump], ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
	
	if (InputActionMap.Contains(EInputKey::MouseLClick))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseLClick], ETriggerEvent::Started, this, &ThisClass::OnMouseLClickTrigger);
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseLClick], ETriggerEvent::Completed, this, &ThisClass::OnMouseLClickComplete);
	}

	if (InputActionMap.Contains(EInputKey::MouseRClick))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseRClick], ETriggerEvent::Started, this, &ThisClass::OnMouseRClickTrigger);
		//EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseRClick], ETriggerEvent::Triggered, this, &ThisClass::OnMouseRClickTrigger);
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseRClick], ETriggerEvent::Completed, this, &ThisClass::OnMouseRClickComplete);
	}

	if (InputActionMap.Contains(EInputKey::LockOn))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::LockOn], ETriggerEvent::Triggered, this, &ThisClass::ToggleLockOn);
	}

	if (InputActionMap.Contains(EInputKey::GrapplingHook))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::GrapplingHook], ETriggerEvent::Triggered, this, &ThisClass::GrapplingHook);
	}

	SetInputContextChange(InputMappingContext);
}

void AUSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget)
	{
		UpdateCameraLockOn(DeltaTime);
	}
}

void AUSPlayer::SetInputContextChange(class UInputMappingContext* MappingContext)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (MappingContext == nullptr)
			return;

		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AUSPlayer::Move(const FInputActionValue& Value)
{
	ClickInputClear();

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	UCapsuleComponent* Capsule = GetCapsuleComponent();

	// 스윙 상태일 때
	if (Capsule && Capsule->IsSimulatingPhysics())
	{
		FVector ForwardDir = GetActorForwardVector();
		FVector RightDir = GetActorRightVector();
		float ForcePower = 100000.f;

		FVector SwingForce = (ForwardDir * MovementVector.X + RightDir * MovementVector.Y) * ForcePower;

		Capsule->AddForce(SwingForce);

	}
	// 클라이밍 상태
	else if (ClimbingComponent && ClimbingComponent->IsClimbing())
	{
		ClimbingComponent->ClimbingUp();
		AddMovementInput(GetActorUpVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);

		if (MovementVector.Y < 0)
			ClimbingComponent->ClimbingCornerLeft();

		if (MovementVector.Y > 0)
			ClimbingComponent->ClimbingCornerRight();
	}
	// 평상시
	else
	{
		
		FRotator AimRotation = GetBaseAimRotation();
		FRotator ActorRotation =  GetActorRotation();
		// 이동각도 - 현재각도를 구해서 이동해야하는 각도를 구함, 그중에서 Z값만을 확인
		FRotator DeltaRotation = AimRotation - ActorRotation;
		//UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : %f"), DeltaRotation.Yaw);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AUSPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

	//UE_LOG(LogTemp, Warning, TEXT("Look %s"), *LookAxisVector.ToString());
}

void AUSPlayer::ClickMove()
{
	APlayerController* pController = Cast<APlayerController>(GetController());
	if (pController == nullptr)
		return;

	FHitResult Hit;
	bool bHitSuccessful = pController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, Hit.Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(pController, nullptr, Hit.Location, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AUSPlayer::Jump()
{
	if (ClimbingComponent && ClimbingComponent->IsClimbing())
	{
		// 등반 반대 방향
		FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		ForwardVector.Z = 0;
		ForwardVector *= -50;

		LaunchCharacter(ForwardVector, false, false);

		ClimbingComponent->ClimbingClear();


		return;
	}
	else if (ParkourComponent && ParkourComponent->IsParkourable())
	{
		ParkourComponent->Start();
	}
	else if (GrapplingHookComponent && GrapplingHookComponent->IsRopeAction())
	{
		if (GrapplingHookComponent->IsAiming() == false)
		{
			GrapplingHookComponent->SwingEnd();
		}
	}
	Super::Jump();
}

void AUSPlayer::ClickInputClear()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->StopMovement();
	}
}

UCameraComponent* AUSPlayer::GetFollowCamera()
{
	return FollowCamera;
}

void AUSPlayer::LockOnTarget()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + (FollowCamera->GetForwardVector() * 10000.0f); // 10,000 유닛 앞까지 라인 트레이스
	
	FHitResult HitResult;
	if (HitCheck(Start, End, HitResult, true, 5.0f, false))
	{
		// 히트된 액터가 캐릭터인지 확인합니다.
		AActor* NearestTarget = HitResult.GetActor();
		if (NearestTarget && NearestTarget->IsA<ACharacter>())
		{
			CurrentTarget = NearestTarget;
			bUseControllerRotationYaw = true;
		}
	}
}

void AUSPlayer::UnlockTarget()
{
	CurrentTarget = nullptr;
	bUseControllerRotationYaw = false;
}

void AUSPlayer::ToggleLockOn()
{
	if (CurrentTarget)
	{
		UnlockTarget();
	}
	else
	{
		LockOnTarget();
	}
}

void AUSPlayer::GrapplingHook()
{
	if (GrapplingHookComponent && GrapplingHookComponent->IsRopeAction())
	{
		GrapplingHookComponent->HookStart();
	}
}

void AUSPlayer::UpdateCameraLockOn(float DeltaTime)
{
	if (!CurrentTarget)
	{
		return;
	}


	// 타겟 위치와 현재 위치를 기반으로 방향 벡터 계산
	FVector TargetLocation = CurrentTarget->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation(); // 방향 벡터를 회전값으로 변환

	// 현재 제어 회전 값을 가져오기
	FRotator CurrentRotation = GetControlRotation();

	// TargetRotation에서 Pitch 값만 사용하고, Yaw와 Roll 값은 유지
	FRotator NewRotation(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);

	// 부드럽게 회전하기 위해 보간
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, 5.0f);

	// 컨트롤러의 회전값 설정
	GetController()->SetControlRotation(SmoothRotation);

}

void AUSPlayer::AddPartner()
{
	if (UWorld* World = GetWorld())
	{
		if (PartnerBPClass != nullptr)
		{
			// 액터 스폰
			AUSPartner* NewPartner = World->SpawnActor<AUSPartner>(PartnerBPClass, GetActorLocation(), FRotator::ZeroRotator);
			if (NewPartner)
			{
				// 배열에 추가
				USPartnerList.Add(NewPartner);
				UE_LOG(LogTemp, Warning, TEXT("Partner %s added to list"), *NewPartner->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load BP_USPartner"));
		}
	}
}

void AUSPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ASCComponent == nullptr)
		return;

	ASCComponent->GenericGameplayEventCallbacks.FindOrAdd(USTAG_CHARACTER_EQUIP_WEAPON).AddUObject(this, &AUSPlayer::EquipWeaponCallBack);
	ASCComponent->GenericGameplayEventCallbacks.FindOrAdd(USTAG_CHARACTER_EQUIP_SHIELD).AddUObject(this, &AUSPlayer::EquipShieldCallBack);

	const UUSChracterAttributeSet* CurrentAttributeSet = ASCComponent->GetSet<UUSChracterAttributeSet>();
	if (CurrentAttributeSet)
	{
		CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
	}
}

void AUSPlayer::EquipWeaponCallBack(const FGameplayEventData* EventData)
{
	if (EventData == nullptr)
		return;
	if (EquipSword == nullptr)
		return;

	const UStaticMeshComponent* Equip = Cast<UStaticMeshComponent>(EventData->OptionalObject.Get());

	EquipSword->SetStaticMesh(Equip->GetStaticMesh());
	EquipSword->SetupAttachment(GetMesh(), TEXT("Sword_Holder"));
}

void AUSPlayer::EquipShieldCallBack(const FGameplayEventData* EventData)
{
	if (EventData == nullptr)
		return;
	if (EquipShield == nullptr)
		return;

	const UStaticMeshComponent* Equip = Cast<UStaticMeshComponent>(EventData->OptionalObject.Get());

	EquipShield->SetStaticMesh(Equip->GetStaticMesh());
}

void AUSPlayer::OnMouseRClickTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMouseRClickTrigger"));
	
	MoveSetting(CharacterInputState);
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		if (ASCComponent->HasMatchingGameplayTag(USTAG_CHARACTER_STATE_SHIELD_ACTIVE))
			return;
		UE_LOG(LogTemp, Warning, TEXT("ShieldActivated"));
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, USTAG_INPUT_SHIELD_ACTIVE, PayloadData);
	}
	else if (CharacterInputState == ECharacterInputState::GrapplingTargetOn)
	{
		SetCharacterInputState(ECharacterInputState::GrapplingTargetOn);
		MoveSetting(CharacterInputState);
		if (GrapplingHookComponent)
		{
			GrapplingHookComponent->BeginAim();
		}
	}
}

void AUSPlayer::OnMouseRClickComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMouseRClickComplete"));
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShieldDeactivated"));
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, USTAG_INPUT_SHIELD_DEACTIVE, PayloadData);
		SetCharacterInputState(ECharacterInputState::Weapon);
	}
	else if (CharacterInputState == ECharacterInputState::GrapplingTargetOn)
	{
		if (GrapplingHookComponent)
		{
			GrapplingHookComponent->EndAim();
		}
		SetCharacterInputState(ECharacterInputState::None);
	}
	
	MoveSetting(CharacterInputState);
}

void AUSPlayer::OnMouseLClickTrigger()
{
	
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		if (ASCComponent == nullptr)
			return;
		FGameplayAbilitySpec* Spec = nullptr;
		if (GetCharacterMovement()->IsFalling())
		{
			Spec = ASCComponent->FindAbilitySpecFromInputID(10);
		}
		else
		{
			Spec = ASCComponent->FindAbilitySpecFromInputID(2);
		}

		if (Spec)
		{
			Spec->InputPressed = true;
			if (Spec->IsActive())
			{
				ASCComponent->AbilitySpecInputPressed(*Spec);
			}
			else
			{
				ASCComponent->TryActivateAbility(Spec->Handle);
			}
		}
	}
}

void AUSPlayer::OnMouseLClickComplete()
{

}

void AUSPlayer::OnOutOfHealth()
{
	SetDeathEvent();
}

void AUSPlayer::MoveSetting(ECharacterInputState CharacterState)
{
	if (CharacterState == ECharacterInputState::None)
	{
		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraBoom->bUsePawnControlRotation = true;
		FollowCamera->bUsePawnControlRotation = false;
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	else if (CharacterState == ECharacterInputState::GrapplingTargetOn)
	{
		bUseControllerRotationYaw = true; // 카메라에 맞춰서 캐릭터를 회전하기 위해서
		GetCharacterMovement()->bOrientRotationToMovement = false; // 이동시 카메라를 바라보는 방향으로 고정
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
	else if (CharacterState == ECharacterInputState::Weapon)
	{
		bUseControllerRotationYaw = true; // 카메라에 맞춰서 캐릭터를 회전하기 위해서
		GetCharacterMovement()->bOrientRotationToMovement = false; // 이동시 카메라를 바라보는 방향으로 고정
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
	}
	else if (CharacterState == ECharacterInputState::GrapplingSwing)
	{
		bUseControllerRotationYaw = true; // 카메라에 맞춰서 캐릭터를 회전하기 위해서
		GetCharacterMovement()->bOrientRotationToMovement = false; // 이동시 카메라를 바라보는 방향으로 고정
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
}

void AUSPlayer::SetCharacterInputState(ECharacterInputState InputState)
{
	CharacterInputState = InputState;
}

