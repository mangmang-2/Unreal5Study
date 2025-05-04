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
		// �̸��� ViewType ���� ���Խ��� �� SceneCapture ������Ʈ�� ������ �̸��� �����մϴ�.
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
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseRClick], ETriggerEvent::Completed, this, &ThisClass::OnMouseRClickComplete);
	}

	if (InputActionMap.Contains(EInputKey::LockOn))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::LockOn], ETriggerEvent::Triggered, this, &ThisClass::ToggleLockOn);
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

	if (ClimbingComponent && ClimbingComponent->IsClimbing())
	{
		ClimbingComponent->ClimbingUp();
		AddMovementInput(GetActorUpVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);

		if (MovementVector.Y < 0)
			ClimbingComponent->ClimbingCornerLeft();

		if (MovementVector.Y > 0)
			ClimbingComponent->ClimbingCornerRight();
	}
	else
	{
		
		FRotator AimRotation = GetBaseAimRotation();
		FRotator ActorRotation =  GetActorRotation();
		// �̵����� - ���簢���� ���ؼ� �̵��ؾ��ϴ� ������ ����, ���߿��� Z������ Ȯ��
		FRotator DeltaRotation = AimRotation - ActorRotation;
		//UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : %f"), DeltaRotation.Yaw);


		//if (DeltaRotation.Yaw < 0)
		//{
		//	// ���������� ���� �̺�Ʈ
		//	//UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : Right"));
		//	K2_RotateRight(DeltaRotation.Yaw, ActorRotation);
		//}
		//else
		//{
		//	// �������� ���� �̺�Ʈ
		//	//UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : Left"));
		//	K2_RotateLeft(DeltaRotation.Yaw, ActorRotation);
		//}
		

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
		// ��� �ݴ� ����
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
	FVector End = Start + (FollowCamera->GetForwardVector() * 10000.0f); // 10,000 ���� �ձ��� ���� Ʈ���̽�
	
	FHitResult HitResult;
	if (HitCheck(Start, End, HitResult, true, 5.0f, false))
	{
		// ��Ʈ�� ���Ͱ� ĳ�������� Ȯ���մϴ�.
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

void AUSPlayer::UpdateCameraLockOn(float DeltaTime)
{
	if (!CurrentTarget)
	{
		return;
	}


	// Ÿ�� ��ġ�� ���� ��ġ�� ������� ���� ���� ���
	FVector TargetLocation = CurrentTarget->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation(); // ���� ���͸� ȸ�������� ��ȯ

	// ���� ���� ȸ�� ���� ��������
	FRotator CurrentRotation = GetControlRotation();

	// TargetRotation���� Pitch ���� ����ϰ�, Yaw�� Roll ���� ����
	FRotator NewRotation(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);

	// �ε巴�� ȸ���ϱ� ���� ����
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, 5.0f);

	// ��Ʈ�ѷ��� ȸ���� ����
	GetController()->SetControlRotation(SmoothRotation);

}

void AUSPlayer::AddPartner()
{
	if (UWorld* World = GetWorld())
	{
		if (PartnerBPClass != nullptr)
		{
			// ���� ����
			AUSPartner* NewPartner = World->SpawnActor<AUSPartner>(PartnerBPClass, GetActorLocation(), FRotator::ZeroRotator);
			if (NewPartner)
			{
				// �迭�� �߰�
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
	SetCharacterInputState(ECharacterInputState::Grappling);
	MoveSetting(CharacterInputState);
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		if (ASCComponent->HasMatchingGameplayTag(USTAG_CHARACTER_STATE_SHIELD_ACTIVE))
			return;
		UE_LOG(LogTemp, Warning, TEXT("ShieldActivated"));
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, USTAG_INPUT_SHIELD_ACTIVE, PayloadData);
	}
	else if (CharacterInputState == ECharacterInputState::Grappling)
	{
		
		if (GrapplingHookComponent)
		{
			GrapplingHookComponent->HookStart();
		}
	}
}

void AUSPlayer::OnMouseRClickComplete()
{
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShieldDeactivated"));
		FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, USTAG_INPUT_SHIELD_DEACTIVE, PayloadData);
	}
	else if (CharacterInputState == ECharacterInputState::Grappling)
	{
		if (GrapplingHookComponent)
		{
			GrapplingHookComponent->HookEnd();
		}
	}
	SetCharacterInputState(ECharacterInputState::None);
	MoveSetting(CharacterInputState);
}

void AUSPlayer::OnMouseLClickTrigger()
{
	if (CharacterInputState == ECharacterInputState::Weapon)
	{
		if (ASCComponent == nullptr)
			return;
		FGameplayAbilitySpec* Spec = ASCComponent->FindAbilitySpecFromInputID(2);
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
	if (GrapplingHookComponent)
	{
		GrapplingHookComponent->HookEnd();
	}
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
	else if (CharacterState == ECharacterInputState::Grappling)
	{
		bUseControllerRotationYaw = true; // ī�޶� ���缭 ĳ���͸� ȸ���ϱ� ���ؼ�
		GetCharacterMovement()->bOrientRotationToMovement = false; // �̵��� ī�޶� �ٶ󺸴� �������� ����
		GetCharacterMovement()->MaxWalkSpeed = 500.f;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
	else if (CharacterState == ECharacterInputState::Weapon)
	{
		bUseControllerRotationYaw = true; // ī�޶� ���缭 ĳ���͸� ȸ���ϱ� ���ؼ�
		GetCharacterMovement()->bOrientRotationToMovement = false; // �̵��� ī�޶� �ٶ󺸴� �������� ����
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
}

void AUSPlayer::SetCharacterInputState(ECharacterInputState InputState)
{
	CharacterInputState = InputState;
}

