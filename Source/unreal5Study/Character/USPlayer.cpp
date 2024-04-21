// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/USCameraData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "../MiniView/MiniViewComponent.h"
#include "Components/CapsuleComponent.h"

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

	if (InputActionMap.Contains(EInputKey::CameraChange))
	{
		EnhancedInputComponent->BindAction(InputActionMap[EInputKey::CameraChange], ETriggerEvent::Triggered, this, &ThisClass::CameraChange);
	}

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

}

void AUSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bIsClimbingUp)
		return;
	// 이게맞는가??
	FVector StartPoint = GetCapsuleComponent()->GetComponentLocation();

	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector() * 45;
	FVector EndPoint = StartPoint + ForwardVector;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); // 이 액터는 트레이스에서 제외
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Visibility,
		QueryParams
	);

	// 라인 트레이스 경로를 디버그용으로 그리기
	DrawDebugLine(
		GetWorld(),
		StartPoint,
		EndPoint,
		FColor::Red,
		false,  // 지속적으로 그릴 것인지 여부
		1.0f,   // 지속 시간
		0,      // DepthPriority
		1.0f    // 선의 두께
	);

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Hit: %s"), bHit ? *FString("true") : *FString("false")));

	if (bHit)
	{
		// 머리 부분이 벽의 끝지점에 있는지 확인
		FHitResult HitResult2;
		FVector HeadPoint = StartPoint + FVector(0, 0, 110) + ForwardVector;
		bIsClimbingEdge = !GetWorld()->LineTraceSingleByChannel(
			HitResult2,
			StartPoint,
			HeadPoint,
			ECC_Visibility,
			QueryParams
		);

		if (bIsClimbingEdge)
		{
			bIsClimbingUp = true;
			// 올라가는 몽타주 실행
			// 몽타주 실행중엔 여기 안타도록 필요


			// 올라가는 행동 종료
			bIsClimbing = false;
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
			CharMoveComp->bOrientRotationToMovement = true;
		}
		DrawDebugLine(
			GetWorld(),
			StartPoint,
			HeadPoint,
			FColor::Blue,
			false,  // 지속적으로 그릴 것인지 여부
			1.0f,   // 지속 시간
			0,      // DepthPriority
			1.0f    // 선의 두께
		);


		FVector Normal = HitResult.Normal; // 충돌 지점의 법선 벡터
		FRotator Rotation = FRotationMatrix::MakeFromX(Normal).Rotator();

		double NewYawValue = Rotation.Yaw + 180;
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYawValue, CurrentRotation.Roll);

		SetActorRotation(NewRotation);

		// 여기서 부터 한번만
		if (bIsClimbing == false)
		{
			bIsClimbing = true;

			GetMovementComponent()->Velocity = FVector(0, 0, 0);

			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
			CharMoveComp->bOrientRotationToMovement = false;
		}
		
	}
	else
	{
		if (bIsClimbing)
		{
			bIsClimbing = false;
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
			CharMoveComp->bOrientRotationToMovement = true;;
		}	
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

	if (bIsClimbing)
	{
		if (bIsClimbingEdge == false)
		{
			AddMovementInput(GetActorUpVector(), MovementVector.X);
			AddMovementInput(GetActorRightVector(), MovementVector.Y);
		}
		else // 이떄 한번 크게 올려줘야하는데.. 일단 모션이 생기면...
		{
		}
	}
	else
	{
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
	if (bIsClimbing)
	{
		// 등반 반대 방향
		FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		ForwardVector.Z = 0;
		ForwardVector *= -50;

		LaunchCharacter(ForwardVector, false, false);
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
