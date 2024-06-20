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

AUSPlayer::AUSPlayer()
{
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;

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
}

void AUSPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetupCemeraSprigArm();
	CameraChange();
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		HUDWidget->AddToViewport();
	}

	AddPartner();
}

// 값 변경이 아니라 세팅값을 둔 여러게의 스프링암을 만들고 교체하는 형식으로 바껴야할듯
void AUSPlayer::SetViewData(const UUSCameraData* CameraData)
{
	if (CameraData == nullptr)
		return;

	bUseControllerRotationYaw = CameraData->bUseControllerRotationYaw;
	if (bUseControllerRotationYaw == false)
	{
		Controller->SetControlRotation(FRotator::ZeroRotator);
	}
	
	SetInputContextChange(CameraData->InputMappingContext);

	
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
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::MouseLClick], ETriggerEvent::Triggered, this, &ThisClass::NormalAttack);
	}

	if (InputActionMap.Contains(EInputKey::LockOn))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::LockOn], ETriggerEvent::Triggered, this, &ThisClass::ToggleLockOn);
	}
}

void AUSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget)
	{
		UpdateCameraLockOn(DeltaTime);
	}

	
}

void AUSPlayer::SetInputContextChange(class UInputMappingContext* InputMappingContext)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (InputMappingContext == nullptr)
			return;

		UInputMappingContext* NewMappingContext = InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
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
		// 이동각도 - 현재각도를 구해서 이동해야하는 각도를 구함, 그중에서 Z값만을 확인
		FRotator DeltaRotation = AimRotation - ActorRotation;
		UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : %f"), DeltaRotation.Yaw);


		if (DeltaRotation.Yaw < 0)
		{
			// 오른쪽으로 가는 이벤트
			UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : Right"));
		}
		else
		{
			// 왼쪽으로 가는 이벤트
			UE_LOG(LogTemp, Warning, TEXT("DeltaRotation : Left"));
		}
		
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

void AUSPlayer::CameraChange()
{
	// 현재 시점을 다음 시점으로 업데이트
	CurrentViewType = GetNextViewType(CurrentViewType);

	// 새로운 시점에 맞게 카메라 데이터 설정
	if (CameraTypeMap[CurrentViewType])
	{
		SetViewData(CameraTypeMap[CurrentViewType]); // 이걸 카메라 변경이라고 봐야하나...하는일
	}
	
	// 스프링암에 붙임
	if (CemeraSprigArm[CurrentViewType])
	{
		FollowCamera->AttachToComponent(CemeraSprigArm[CurrentViewType], FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	}
		
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


EViewType AUSPlayer::GetNextViewType(EViewType CurrentView)
{
	switch (CurrentView)
	{
	case EViewType::None: return EViewType::FirstPerson;
	case EViewType::FirstPerson: return EViewType::ThirdPerson;
	case EViewType::ThirdPerson: return EViewType::TopDown;
	case EViewType::TopDown: return EViewType::FirstPerson;
	default: return EViewType::None; // 기본값 처리
	}
}

void AUSPlayer::SetupCemeraSprigArm()
{
	for (uint8 viewtype = (uint8)EViewType::None; viewtype < (uint8)EViewType::Max; ++viewtype)
	{
		SetCameraSprigArm( static_cast<EViewType>(viewtype));
	}
}

void AUSPlayer::SetCameraSprigArm(EViewType ViewType)
{
	if (CameraTypeMap.Contains(ViewType) == false)
		return;

	const UUSCameraData* CameraData = CameraTypeMap[ViewType];
	if (CameraData == nullptr)
		return;

	//TObjectPtr<USpringArmComponent> SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName(*FString::Printf(TEXT("CameraBoom%d"), static_cast<int32>(ViewType))));
	TObjectPtr<USpringArmComponent> SpringArm = NewObject<USpringArmComponent>(this, FName(*FString::Printf(TEXT("CameraBoom%d"), static_cast<int32>(ViewType))));
	SpringArm->RegisterComponent();

	SpringArm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	SpringArm->TargetArmLength = CameraData->TargetArmLength;
	SpringArm->SetRelativeRotation(CameraData->RelativeRotation);
	SpringArm->SetRelativeLocation(CameraData->RelativeLocation);
	SpringArm->bUsePawnControlRotation = CameraData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = CameraData->bInheritPitch;
	SpringArm->bInheritYaw = CameraData->bInheritYaw;
	SpringArm->bInheritRoll = CameraData->bInheritRoll;
	SpringArm->bDoCollisionTest = CameraData->bDoCollisionTest;

	CemeraSprigArm.Add(ViewType, SpringArm);

	SceneCapture[ViewType]->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
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

