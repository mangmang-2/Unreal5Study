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
#include "MotionWarpingComponent.h"

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

	if (bIsClimbingFalling || bIsClimbingUp)
		return;

	FVector UpVector = GetCapsuleComponent()->GetUpVector() * 60;
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector() * 80;

	FVector StartPoint = GetCapsuleComponent()->GetComponentLocation();
	FVector MiddleEndPoint = StartPoint + ForwardVector;

	FHitResult HitResultMiddle;
	bool bHitMiddle = HitCheck(StartPoint, MiddleEndPoint, HitResultMiddle, false);

	if (bHitMiddle == false && bIsClimbing)
	{
		ClimbingClear();
	}

	if (GetCharacterMovement()->IsFalling())
	{
		if (bHitMiddle && bIsClimbing == false)
		{
			bIsClimbing = true;
			GetMovementComponent()->Velocity = FVector(0, 0, 0);
			
			UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (CharMoveComp)
			{
				CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
				CharMoveComp->bOrientRotationToMovement = false;
			}
		}
		
	}

	if (bIsClimbing)
	{
		if (bHitMiddle)
		{
			float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();

			SetActorLocation(HitResultMiddle.Normal * CapsuleRadius + HitResultMiddle.Location);

			FRotator Rotation = FRotationMatrix::MakeFromX(HitResultMiddle.Normal).Rotator();

			double NewYawValue = Rotation.Yaw + 180;
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYawValue, CurrentRotation.Roll);

			SetActorRotation(NewRotation);
		}
	}
	
	// 머리 확인
	FVector HeadStartPoint = StartPoint + UpVector;
	FVector HeadEndPoint = StartPoint + ForwardVector + UpVector;
	FHitResult HitResultHead;
	bool bHitHead = HitCheck(HeadStartPoint, HeadEndPoint, HitResultHead, false);

	if(bIsClimbing && bHitHead == false)
	{
		FVector Offset = GetCapsuleComponent()->GetForwardVector() * 10;
		FVector OffsetStart = HeadStartPoint - UpVector;
		for (int i = 0; i < 9; ++i)
		{
			FVector SPoint = OffsetStart + Offset * i;
			FVector EPoint = SPoint + UpVector * 3;

			if (HitCheck(EPoint, SPoint,  HitResultHead, false))
				break;
		}

		// 캐릭터가 들어갈 수 있는 곳인지 확인
		float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		FVector CapsuleOrigin = HitResultHead.ImpactPoint + GetCapsuleComponent()->GetUpVector() * (CapsuleHalfHeight +1);
		FHitResult CapsuleRadiusHitResult;
		if (CapsuleHitCheck(CapsuleOrigin, CapsuleRadius, CapsuleHalfHeight, CapsuleRadiusHitResult) == false)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && ClimbingTopMontage && bIsClimbingUp == false)
			{
				MotionWarping->AddOrUpdateWarpTargetFromLocation(TEXT("Warp1"), HitResultHead.ImpactPoint);

				bIsClimbingUp = true;
				AnimInstance->Montage_Play(ClimbingTopMontage, 1.0);

				float MontageLength = ClimbingTopMontage->GetPlayLength();
		 
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
						ClimbingClear();
					})
					, MontageLength, false);
			}
		}
	}

	/*FVector Offset = GetCapsuleComponent()->GetRightVector() * 10;
	FVector OffsetStart = StartPoint -(Offset * 4) + GetCapsuleComponent()->GetUpVector() * 20;
	for (int i = 0; i < 9; ++i)
	{
		FVector SPoint = OffsetStart + Offset * i;
		FVector EPoint = SPoint + ForwardVector;

		HitCheck(SPoint, EPoint, HitResultHead, false);
	}*/

	

	//{
	//	// 캐릭터 캡슐
	//	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	//	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	//	FVector CapsuleOrigin = StartPoint;
	//	FHitResult CapsuleRadiusHitResult;
	//	CapsuleHitCheck(CapsuleOrigin, CapsuleRadius, CapsuleHalfHeight, CapsuleRadiusHitResult);
	//}




	//FHitResult HitResult2;
	//static int aa = 100;
	//FVector StartPoint2 = GetCapsuleComponent()->GetComponentLocation() + GetCapsuleComponent()->GetForwardVector() * 35 + GetCapsuleComponent()->GetUpVector() * aa;
	//FVector EndPoint2 = StartPoint2 - GetCapsuleComponent()->GetUpVector() * 35;
	//bool bHit2 = HitCheck(StartPoint2, EndPoint2, HitResult2, true);

	//if (bHit)
	//{		
	//	if (bHit2) // 충분히 위로 올라왔다면 오르는 애니메이션
	//	{
	//		UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance();
	//		if (AnimInstance && ClimbingTopMontage)
	//		{
	//			AnimInstance->Montage_Play(ClimbingTopMontage, 1.0);
	//			bIsClimbingUp = true;

	//			return;
	//		}
	//	}

	//	FVector Normal = HitResult.Normal; // 충돌 지점의 법선 벡터
	//	FRotator Rotation = FRotationMatrix::MakeFromX(Normal).Rotator();

	//	double NewYawValue = Rotation.Yaw + 180;
	//	FRotator CurrentRotation = GetActorRotation();
	//	FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYawValue, CurrentRotation.Roll);

	//	SetActorRotation(NewRotation);
	//		
	//	// 여기서 부터 한번만
	//	if (bIsClimbing == false)
	//	{
	//		//FVector CurrentLocation = GetActorLocation();
	//		//CurrentLocation += (GetCapsuleComponent()->GetForwardVector() * 2);
	//		//SetActorLocation(CurrentLocation);

	//		bIsClimbing = true;
	//		//ClimbingSearch = 100;
	//		GetMovementComponent()->Velocity = FVector(0, 0, 0);

	//		UCharacterMovementComponent* CharMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	//		if(CharMoveComp)
	//		{
	//			CharMoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
	//			CharMoveComp->bOrientRotationToMovement = false;
	//		}
	//	}
	//	
	//}
	//else
	//{
	//	if (bHit2) // 충분히 위로 올라왔다면 오르는 애니메이션
	//	{
	//		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//		if (AnimInstance && ClimbingTopMontage)
	//		{
	//			AnimInstance->Montage_Play(ClimbingTopMontage, 1.0);
	//			bIsClimbingUp = true;

	//			return;
	//		}
	//	}
	//	else
	//	{
	//		if (bIsClimbing)
	//		{
	//			ClimbingClear();
	//		}
	//	}
	//}
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
		AddMovementInput(GetActorUpVector(), MovementVector.X);

		AddMovementInput(GetActorRightVector(), MovementVector.Y);
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

		ClimbingClear();


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

