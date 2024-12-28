// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USCropoutPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "USVillagerInterface.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "../Interactable/USInteractable.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/BoxComponent.h"
#include "GameMode/USCropoutGameMode.h"

// Sets default values
AUSCropoutPlayer::AUSCropoutPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
	Cursor->SetupAttachment(RootComponent);
	Cursor->SetRelativeLocation(FVector(0.0f, 0.0f, 0.5f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1139.963867f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;
	SpringArm->SocketOffset = FVector(-300.0, 0, 80.0);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/Study/Cropout/Data/C_Zoom.C_Zoom"));

	if (Curve.Succeeded())
	{
		ZoomCurve = Curve.Object;
	}

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	SpawnOverlay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnOverlay"));
	SpawnOverlay->SetVisibility(false);
	SpawnOverlay->SetCanEverAffectNavigation(false);

	
}

// Called when the game starts or when spawned
void AUSCropoutPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateZoom();

	FRotator RotationDelta(0.f, -45.0f, 0.0f);
	AddActorLocalRotation(RotationDelta);
}

// Called every frame
void AUSCropoutPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveTracking();
	
}

// Called to bind functionality to input
void AUSCropoutPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		if (BaseInputMappingContext)
		{
			Subsystem->AddMappingContext(BaseInputMappingContext, 0);
		}

		if (VillagerMappingContext)
		{
			Subsystem->AddMappingContext(VillagerMappingContext, 0);
		}
	}

}

void AUSCropoutPlayer::UpdateZoom()
{
	ZoomValue = FMath::Clamp((ZoomDirection * 0.01) + ZoomValue, 0, 1);

	float Alpha = ZoomCurve->GetFloatValue(ZoomValue);

	float TargetArmLength = UKismetMathLibrary::Lerp(800.0f, 40000.0f, Alpha);
	SpringArm->TargetArmLength = TargetArmLength;

	float TargetRotation = UKismetMathLibrary::Lerp(-40.0f, -55.0f, Alpha);
	SpringArm->SetRelativeRotation(FRotator(TargetRotation, 0.0f, 0.0f));

	float MaxSpeed = UKismetMathLibrary::Lerp(1000.0f, 6000.0f, Alpha);
	FloatingPawnMovement->MaxSpeed = MaxSpeed;

	Dof();

	float FieldOfView = UKismetMathLibrary::Lerp(20.0f, 15.0f, Alpha);
	Camera->SetFieldOfView(FieldOfView);

}

void AUSCropoutPlayer::Dof()
{
	// 포스트 프로세스 설정 객체를 생성하고 값을 설정
	FPostProcessSettings PostProcessSettings;
	PostProcessSettings.bOverride_DepthOfFieldFstop = true;    // 조리개 값 오버라이드 설정
	PostProcessSettings.DepthOfFieldFstop = 3.0f;              // 조리개 값 설정 (f-stop)

	PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true; // 센서 너비 오버라이드 설정
	PostProcessSettings.DepthOfFieldSensorWidth = 150.0f;         // 센서 너비 설정 (mm 단위)

	PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true; // 초점 거리 오버라이드 설정
	PostProcessSettings.DepthOfFieldFocalDistance = SpringArm->TargetArmLength; // SpringArm 길이를 초점 거리로 설정

	// 카메라에 포스트 프로세스 설정을 적용합니다.
	Camera->PostProcessSettings = PostProcessSettings;
}

void AUSCropoutPlayer::MoveTracking()
{
	FVector PlayerLocation = GetActorLocation();
	FVector NormalizedLocation = PlayerLocation.GetSafeNormal(0.0001f); // Tolerance를 0.0001로 설정
	float LocationLength = PlayerLocation.Size();
	float ScaleValue = FMath::Max(0, (LocationLength - 9000.0f) / 5000.0f);

	FVector WorldDirection = FVector(NormalizedLocation.X, NormalizedLocation.Y, 0) * -1.0f;
	// 이동 입력 추가
	AddMovementInput(WorldDirection, ScaleValue);

	UpdateCursorPosition();

	EdgeMode();

	PositionCollisionOnGroundPlane();

	
}

void AUSCropoutPlayer::EdgeMode()
{
	//FVector WorldDirection;
	//float ScaleValue;

	FVector2D ViewportCenter = GetViewportCenter();

	FVector2D ScreenPos;
	FVector Intersection;
	bool bMousePostion;
	ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);

	FVector Direction = CursorDistFromViewportCenter(ScreenPos - ViewportCenter);
	FTransform ActorTransform = GetActorTransform();
	FVector TransformDirection = UKismetMathLibrary::TransformDirection(ActorTransform, Direction);

	float Strength = 1.0f;
	AddMovementInput(TransformDirection, Strength);
}

void AUSCropoutPlayer::ProjectMouseToGroundPlane(FVector2D& ScreenPosition, FVector& Intersection, bool& bMousePostion)
{
	ScreenPosition = GetMouseViewportPosition(bMousePostion);
	if (bMousePostion == false)
		ScreenPosition = GetViewportCenter();

	Intersection = ProjectScreenPositionToGamePlane(ScreenPosition);
}

FVector2D AUSCropoutPlayer::GetViewportCenter()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return FVector2D::ZeroVector;

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	return FVector2D(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);
}

FVector2D AUSCropoutPlayer::GetMouseViewportPosition(bool& bMousePostion)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return FVector2D::ZeroVector;

	float LocationX, LocationY;
	bMousePostion = PlayerController->GetMousePosition(LocationX, LocationY);

	return FVector2D(LocationX, LocationY);
}

FVector AUSCropoutPlayer::ProjectScreenPositionToGamePlane(FVector2D ScreenPosition)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return FVector::ZeroVector;

	FVector WorldPosition, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection);

	FVector LineEnd = WorldPosition + (WorldDirection * 100000.0f);
	FPlane APlane(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f));

	FVector Intersection;
	float T;
	UKismetMathLibrary::LinePlaneIntersection(WorldPosition, LineEnd, APlane, T, Intersection);

	return Intersection;
}

FVector AUSCropoutPlayer::CursorDistFromViewportCenter(FVector2D ScreenPos)
{
	FVector2D Distance = CalculateEdgeMoveDistance();
	FVector2D OffsetMousePosition = OffsetMousePositionToCreateDeadZone(ScreenPos, Distance);
	FVector2D Adjusted = AdjustForNegativeDirection(ScreenPos);

	return FVector(Adjusted.Y * OffsetMousePosition.Y * -1.0f, Adjusted.X * OffsetMousePosition.X, 0.0f);
}

FVector2D AUSCropoutPlayer::CalculateEdgeMoveDistance()
{
	FVector2D ViewportCenter = GetViewportCenter();

	// 최종 Edge Move Distance 계산
	return FVector2D(ViewportCenter.X - EdgeMoveDistance, ViewportCenter.Y - EdgeMoveDistance);
}

FVector2D AUSCropoutPlayer::OffsetMousePositionToCreateDeadZone(FVector2D ScreenPos, FVector2D Distance)
{
	// 마우스 위치의 절대값 계산
	float AbsMouseX = FMath::Abs(ScreenPos.X);
	float AbsMouseY = FMath::Abs(ScreenPos.Y);

	// X축, Y축에서 Dead Zone을 감안한 위치 계산
	float OffsetX = FMath::Max(0.0f, AbsMouseX - Distance.X) / EdgeMoveDistance;
	float OffsetY = FMath::Max(0.0f, AbsMouseY - Distance.Y) / EdgeMoveDistance;

	return FVector2D(OffsetX, OffsetY);
}

FVector2D AUSCropoutPlayer::AdjustForNegativeDirection(FVector2D InputVector)
{
	float AdjustedX = FMath::Sign(InputVector.X);
	float AdjustedY = FMath::Sign(InputVector.Y);

	return FVector2D(AdjustedX, AdjustedY);
}

void AUSCropoutPlayer::PositionCollisionOnGroundPlane()
{
	FVector2D ScreenPos;
	FVector Intersection;
	bool bMousePostion;
	ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);
	Intersection.Z += 10.0f;

	FHitResult SweepHitResult;
	bool bSweep = true;

	Collision->SetWorldLocation(Intersection, bSweep, &SweepHitResult);
	return;
}

void AUSCropoutPlayer::UpdateCursorPosition()
{
	if (HoverActor)
	{
		// HoverActor의 바운드 가져오기
		FVector Origin, BoxExtent;
		HoverActor->GetActorBounds(true, Origin, BoxExtent);

		FVector NewOrigin(Origin.X, Origin.Y, 20.0f);

		// 2D 벡터 크기 계산
		FVector2D BoxExtent2D(BoxExtent.X, BoxExtent.Y);
		double MaxExtent = UKismetMathLibrary::GetAbsMax2D(BoxExtent2D) / 50.0f;


		// 시간에 따른 Sin 값을 사용하여 Y축 스케일 조정
		double Time = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		double ScaleY = (FMath::Sin(Time * 5.0) * 0.25);

		// 최종 스케일 값 설정
		FVector Scale(MaxExtent + ScaleY + 1.0, MaxExtent + ScaleY + 1.0, 1.0f);
		FRotator Rotation = FRotator::ZeroRotator;
		Target = UKismetMathLibrary::MakeTransform(NewOrigin, Rotation, Scale);
	}
	else
	{
		FVector TargetLocation = Target.GetLocation();
		FVector AdjustedLocation = FVector(TargetLocation.X, TargetLocation.Y, -100.0f);
		FVector2D ScreenPos;
		FVector Intersection;
		bool bMousePostion;
		ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);

		Target.SetScale3D(FVector(2.0f, 2.0f, 1.0f));
		if (bMousePostion)
		{
			Target.SetLocation(Intersection);
		}
		else
		{
			Target.SetLocation(AdjustedLocation);
		}
	}
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

	FTransform CursorTransform = Cursor->GetComponentToWorld();
	FTransform NewTransform = UKismetMathLibrary::TInterpTo(CursorTransform, Target, DeltaTime, 12.0f);
	FVector Location = NewTransform.GetLocation();
	//Location.Z += 0.5;
	NewTransform.SetLocation(Location);
	Cursor->SetWorldTransform(NewTransform);
}

void AUSCropoutPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor)
	{
		if (HoverActor == nullptr)
		{
			HoverActor = OtherActor;

			// 타이머 설정 - ClosestHoverCheck 함수를 주기적으로 호출
			GetWorldTimerManager().SetTimer(
				TimerHandle_ClosestHoverCheck,            // 타이머 핸들
				this,                                     // 호출 대상
				&AUSCropoutPlayer::ClosestHoverCheck,     // 호출할 함수
				0.01f,                                    // 호출 간격 (0.01초)
				true                                      // 반복 여부 (true로 설정하면 반복 호출)
			);
		}

		
	}
}

void AUSCropoutPlayer::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	if (OverlappingActors.IsEmpty())
	{
		GetWorldTimerManager().SetTimerForNextTick(this, &AUSCropoutPlayer::ClearHoverActor);
	}
}
void AUSCropoutPlayer::ClearHoverActor()
{
	HoverActor = nullptr;
}

void AUSCropoutPlayer::ClosestHoverCheck()
{
	TArray<AActor*> OverlappingActors;
	if (Collision)
	{
		Collision->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	}

	if (OverlappingActors.Num() == 0)
	{
		GetWorldTimerManager().PauseTimer(TimerHandle_ClosestHoverCheck);
		ClearHoverActor();
		return;
	}

	float ClosestDistance = FLT_MAX;
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			// 거리 계산
			float Distance = FVector::Dist(Actor->GetActorLocation(), Collision->GetComponentLocation());

			// 가장 가까운 액터를 찾음
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}

	if (ClosestActor != HoverActor)
	{
		HoverActor = ClosestActor;
	}	
}

void AUSCropoutPlayer::BlueprintZoomFunc(float ActionValue)
{
	ZoomDirection = ActionValue;
	UpdateZoom();
	Dof();
}

void AUSCropoutPlayer::BlueprintVillagerModeTriggered()
{
	VillagerAction = HoverActor;
}

void AUSCropoutPlayer::BlueprintVillagerModeStarted()
{
	PositionCheck();
	AActor* OutputActor = VillageOverlapCheck();
	if (OutputActor)
	{
		VillagerSelect(OutputActor);
	}
	else
	{
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DragMoveMappingContext)
			{
				Subsystem->AddMappingContext(DragMoveMappingContext, 0);
			}
		}
	}
}

void AUSCropoutPlayer::BlueprintVillagerModeComplete()
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		if (DragMoveMappingContext)
		{
			FModifyContextOptions Options;
			Options.bIgnoreAllPressedKeysUntilRelease = true;  // 모든 눌린 키를 릴리스할 때까지 무시
			Options.bForceImmediately = true;                  // 즉시 적용
			Options.bNotifyUserSettings = false;               // 사용자 설정 알림 비활성화

			Subsystem->RemoveMappingContext(DragMoveMappingContext, Options);
		}
	}

	if (Selected)
	{
		IUSVillagerInterface* SelectedVillager = Cast<IUSVillagerInterface>(Selected);
		if (SelectedVillager)
		{
			SelectedVillager->Action(VillagerAction);
		}
		VillagerRelease();
	}
	VillagerAction = nullptr;
}

void AUSCropoutPlayer::VillagerRelease()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(UpdatePathTimerHandle))
		GetWorld()->GetTimerManager().PauseTimer(UpdatePathTimerHandle);

	if (NSPath)
	{
		NSPath->DestroyComponent();
		NSPath = nullptr;
	}

	Selected = nullptr;
}

void AUSCropoutPlayer::BlueprintDragModeTriggered()
{
	TrackerMove();
}

void AUSCropoutPlayer::PositionCheck()
{
	FVector2D ScreenPos;
	FVector Intersection;
	bool bMousePostion;
	ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);

	TargetHandle = Intersection;
}

class AActor* AUSCropoutPlayer::VillageOverlapCheck()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APawn::StaticClass());

	if (OverlappingActors.Num() > 0) // 배열의 길이가 0보다 큰지 확인
	{
		AActor* FirstActor = OverlappingActors[0]; // 첫 번째 요소 가져오기
		if (IsValid(FirstActor) && IsValid(HoverActor)) // 첫 번째 요소가 유효한지 확인
		{
			bool Equal = UKismetMathLibrary::EqualEqual_VectorVector(FirstActor->GetActorLocation(), HoverActor->GetActorLocation(), 10.0f);
			if (Equal)
			{
				return FirstActor;
			}
		}
	}
	return nullptr;
}

void AUSCropoutPlayer::VillagerSelect(AActor* SelectedActor)
{
	Selected = SelectedActor;

	if (UNiagaraSystem* NiagaraSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Study/Cropout/VFX/NS_Target.NS_Target"))) // 시스템 템플릿 경로를 지정
	{
		UNiagaraComponent* NSComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			RootComponent, // 부착할 컴포넌트
			NAME_None,     // Attach Point 이름
			FVector::ZeroVector, // 위치
			FRotator::ZeroRotator, // 회전
			EAttachLocation::SnapToTargetIncludingScale, // 위치 타입
			true, // 자동 파괴
			true  // 자동 활성화
		);

		NSPath = NSComponent;

		GetWorldTimerManager().SetTimer(
			UpdatePathTimerHandle,  // 타이머 핸들
			this,                   // 타겟 객체
			&AUSCropoutPlayer::UpdatePath, // 호출할 함수
			0.01f,                  // 시간 간격
			true                    // 반복 여부 (루핑)
		);
	}
}

void AUSCropoutPlayer::UpdatePath()
{
	if (Collision == nullptr || Selected == nullptr)
		return;

	FVector CollisionLocation = Collision->GetComponentLocation();
	FVector SelectedLocation = Selected->GetActorLocation();

	TArray<FVector> PathPoints;

	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), CollisionLocation, SelectedLocation);
	if (NavPath && NavPath->IsValid())
	{
		PathPoints = NavPath->PathPoints;
	}

	if (PathPoints.Num() > 0)
	{
		PathPoints[0] = CollisionLocation;
		PathPoints.Last() = SelectedLocation;
	}

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NSPath, TEXT("TargetPath"), PathPoints);
}

void AUSCropoutPlayer::TrackerMove()
{
	FVector2D ScreenPos;
	FVector Intersection;
	bool bMousePostion;
	ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);

	if (bMousePostion)
	{
		StoredMove = TargetHandle - Intersection - CalculateCameraOffset();

		AddActorWorldOffset(FVector(StoredMove.X, StoredMove.Y, 0));
	}
}

FVector AUSCropoutPlayer::CalculateCameraOffset()
{
	if (!SpringArm || !Camera) 
		return FVector::ZeroVector;

	FVector ForwardVector = SpringArm->GetForwardVector();
	FVector UpVector = SpringArm->GetForwardVector();
	float ArmLength = SpringArm->TargetArmLength;

	FVector ArmOffset = SpringArm->SocketOffset;

	FVector ForwardOffset = ForwardVector * (ArmLength - ArmOffset.X) * -1.0f;
	FVector UpOffset = UpVector * ArmOffset.Z;

	FVector SpringArmWorldLocation = SpringArm->GetComponentLocation();

	FVector Offset = ForwardOffset + UpOffset + SpringArmWorldLocation;

	FVector CameraWorldLocation = Camera->GetComponentLocation();

	FVector FinalOffset = Offset - CameraWorldLocation;

	return FinalOffset;
}


void AUSCropoutPlayer::BeginBuild(TSubclassOf<AActor> TargetClassParam, TMap<enum EResourceType, int32> CostParam)
{
	ResourceCost = CostParam;
	
	FVector SpawnLocation = GetActorLocation();
	TargetClass = TargetClassParam;

	if (TargetActor != nullptr)
	{
		TargetActor->DoDestroy();
		TargetActor = nullptr;
	}

	if (TargetClass)
	{
		FTransform SpawnTransform = GetActorTransform();
		SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		TargetActor = Cast<AUSInteractable>(GetWorld()->SpawnActor<AActor>(
			TargetClass,
			SpawnTransform,
			SpawnParams
		));
		TargetActor->SetEnableGroundBlend(false);
	}

	if (TargetActor)
	{
		TargetActor->PlacementMode();
	}

	CreateBuildOvelay();
}

void AUSCropoutPlayer::CreateBuildOvelay()
{
	if (IsValid(SpawnOverlay) == false)
		return;

	if (TargetActor == nullptr)
		return;

	FVector Origin, BoxExtent;
	TargetActor->GetActorBounds(false, Origin, BoxExtent);
	BoxExtent /= 50.0f;

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FTransform Transform(Rotation, Location, BoxExtent);

	SpawnOverlay->SetVisibility(true);

	SpawnOverlay->AttachToComponent(
		TargetActor->Mesh,
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			true
		)
	);

	UpdateBuildAsset();
}

void AUSCropoutPlayer::UpdateBuildAsset()
{
	if (IsValid(TargetActor) == false)
		return;

	if (MPC_Cropout == nullptr)
		return;
	FVector2D ScreenPos;
	FVector Intersection;
	bool bMousePostion;
	ProjectMouseToGroundPlane(ScreenPos, Intersection, bMousePostion);

	if (bMousePostion == false)
		return;

	FVector CurrentLocation = TargetActor->GetActorLocation();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector NewLocation = UKismetMathLibrary::VInterpTo(
		CurrentLocation,
		Intersection,
		DeltaTime,
		10.0f
	);

	TargetActor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::None);

	TArray<AActor*> OutOverlappingActors;
	TargetActor->GetOverlappingActors(OutOverlappingActors, AUSInteractable::StaticClass());
	if (OutOverlappingActors.Num() == 0)
	{
		CanDrop = CornersInNav();
	}
	else
	{
		CanDrop = false;
	}
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation SpawnPos;
	if (NavSystem->ProjectPointToNavigation(NewLocation, SpawnPos) == false)
	{
		CanDrop = false;
	}

	FLinearColor Color;
	Color.R = NewLocation.X;
	Color.G = NewLocation.Y;
	Color.B = NewLocation.Z;
	Color.A = CanDrop;

	UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(MPC_Cropout);
	if (Instance == nullptr)
		return;

	Instance->SetVectorParameterValue(FName("Target Position"), Color);
}

bool AUSCropoutPlayer::CornersInNav()
{
	if (TargetActor == nullptr)
		return false;

	FVector Origin, BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(TargetActor->Box.Get(), Origin, BoxExtent, SphereRadius);

	TArray<FVector> CornerPositions;
	CornerPositions.Add(Origin + FVector(BoxExtent.X * 1.05f, BoxExtent.Y * 1.05f, 0.0f));
	CornerPositions.Add(Origin + FVector(BoxExtent.X * 1.05f, -BoxExtent.Y * 1.05f, 0.0f));
	CornerPositions.Add(Origin + FVector(-BoxExtent.X * 1.05f, BoxExtent.Y * 1.05f, 0.0f));
	CornerPositions.Add(Origin + FVector(-BoxExtent.X * 1.05f, -BoxExtent.Y * 1.05f, 0.0f));

	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams(FName(TEXT("MultiLineTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);

	bool bOnIsland = true;
	for (const FVector& Positions : CornerPositions)
	{
		FVector Start(Positions.X, Positions.Y, 100.0f);
		FVector End(Positions.X, Positions.Y, -1);

		HitResults.Empty();
		if (GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, TraceParams))
		{
			bool bHitGround = false;

			for (const FHitResult& Hit : HitResults)
			{
				if (Hit.bBlockingHit)
				{
					bHitGround = true;
					break;
				}
			}

			if (!bHitGround)
			{
				bOnIsland = false;
				break;
			}
		}
		else
		{
			bOnIsland = false;
			break;
		}
	}

	return bOnIsland;
}

void AUSCropoutPlayer::BlueprintMuildMoveComplete()
{
	if (IsValid(TargetActor) == false)
		return;
	FVector NewLocation = GetSteppedPosition(TargetActor->GetActorLocation(), 200);
	TargetActor->SetActorLocation(NewLocation);

	UpdateBuildAsset();
}


FVector AUSCropoutPlayer::GetSteppedPosition(const FVector& InputPosition, float StepSize)
{
	float SnappedX = FMath::RoundToFloat(InputPosition.X / StepSize) * StepSize;
	float SnappedY = FMath::RoundToFloat(InputPosition.Y / StepSize) * StepSize;

	return FVector(SnappedX, SnappedY, 0.0f);
}

void AUSCropoutPlayer::SwitchBuildMode(bool BuildMode)
{
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (EnhancedInputSubsystem == nullptr)
		return;

	if (BuildMode)
	{
		EnhancedInputSubsystem->RemoveMappingContext(VillagerMappingContext);
		EnhancedInputSubsystem->AddMappingContext(BuildModeMappingContext, 0);
	}
	else
	{
		EnhancedInputSubsystem->RemoveMappingContext(BuildModeMappingContext);
		EnhancedInputSubsystem->AddMappingContext(VillagerMappingContext, 0);
	}
}

bool AUSCropoutPlayer::SpawnBuildTarget()
{
	if (TargetActor == nullptr)
		return false;

	if (CanDrop)
	{
		if (RemoveResources() == false)
			return false;

		FTransform SpawnTransform = TargetActor->GetActorTransform();
		FVector NewTranslation =  SpawnTransform.GetTranslation();
		if (TargetClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
				TargetClass,
				SpawnTransform,
				SpawnParams
			);

			if (SpawnedActor)
			{
				/*FVector CurrentLocation = SpawnedActor->GetActorLocation();
				CurrentLocation.X += 0.005f;
				SpawnedActor->SetActorLocation(CurrentLocation);*/

				// 진행 상태 설정
				AUSInteractable* Interactable = Cast<AUSInteractable>(SpawnedActor);
				if (Interactable)
				{
					Interactable->SetProgressionsState(0.0f);
				}

				AUSCropoutGameMode* CropoutGameMode = Cast<AUSCropoutGameMode>(UGameplayStatics::GetGameMode(this));
				if (CropoutGameMode)
				{
					CropoutGameMode->UpdateAllInteractables();
				}
			}
		}
	}

	UpdateBuildAsset();
	return true;
}

void AUSCropoutPlayer::RotateSpawn()
{
	if (TargetActor == nullptr)
		return;

	FRotator Rotator1 = TargetActor->GetActorRotation();
	FRotator Rotator2(0.0f, 90.0f, 0.0f);

	TargetActor->SetActorRotation(Rotator1 + Rotator2);
}

void AUSCropoutPlayer::DestroyTargetActor()
{
	if (TargetActor)
	{
		TargetActor->DoDestroy();
		TargetActor = nullptr;
	}
	if (SpawnOverlay)
	{
		SpawnOverlay->SetVisibility(false);
	}
}

bool AUSCropoutPlayer::RemoveResources()
{
	IUSResourceInterface* GameMode = Cast<IUSResourceInterface>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
		return false;

	for (const auto& ResourcePair : ResourceCost)
	{
		EResourceType ResourceKey = ResourcePair.Key;
		int32 ResourceAmount = ResourcePair.Value;
		int32 Cost = GameMode->GetCurrentResources(ResourceKey);
		if (Cost < ResourceAmount)
		{
			GameMode->RemoveCurrentUILayer();
			TargetActor->DoDestroy();
			TargetActor = nullptr;
			SpawnOverlay->SetVisibility(false);
			return false;
		}
	}

	for (const auto& ResourcePair : ResourceCost)
	{
		EResourceType ResourceKey = ResourcePair.Key;
		int32 ResourceAmount = ResourcePair.Value;

		if (GameMode)
		{
			GameMode->RemoveTargetResource(ResourceKey, ResourceAmount);
		}
	}

	return true;
}