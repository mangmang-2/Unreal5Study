// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/USCameraData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AUSPlayer::AUSPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AUSPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCameraData(CameraTypeMap[ECameraType::ThirdPerson]);
	SetInputControll();
}

void AUSPlayer::SetCameraData(const UUSCameraData* CameraData)
{
	if (CameraData == nullptr)
		return;

	CameraBoom->TargetArmLength = CameraData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CameraData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CameraData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CameraData->bInheritPitch;
	CameraBoom->bInheritYaw = CameraData->bInheritYaw;
	CameraBoom->bInheritRoll = CameraData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CameraData->bDoCollisionTest;
}

void AUSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Move], ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(InputActionMap[EInputKey::Look], ETriggerEvent::Triggered, this, &ThisClass::Look);
}

void AUSPlayer::SetInputControll()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
}

void AUSPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AUSPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
