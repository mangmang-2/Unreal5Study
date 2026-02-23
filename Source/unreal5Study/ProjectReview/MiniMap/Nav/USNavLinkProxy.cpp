// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Nav/USNavLinkProxy.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavLinkCustomComponent.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "../../../Character/USCharacterBase.h"

AUSNavLinkProxy::AUSNavLinkProxy()
{
    // SmartLink 활성화 (OnSmartLinkReached 이벤트 발생 조건)
    SetSmartLinkEnabled(true);

    // NewObject 대신 CreateDefaultSubobject 사용
    TriggerMeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMeshLeft"));
    TriggerMeshLeft->SetupAttachment(RootComponent);

    TriggerMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMeshRight"));
    TriggerMeshRight->SetupAttachment(RootComponent);

    // 화살표 컴포넌트 생성
    LaunchArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchArrow"));
    LaunchArrow->SetupAttachment(RootComponent);
    LaunchArrow->SetHiddenInGame(true);          // 게임 중엔 숨김
    LaunchArrow->bIsScreenSizeScaled = false;
    LaunchArrow->ArrowSize = 2.0f;
    LaunchArrow->ArrowColor = FColor::Yellow;
}

void AUSNavLinkProxy::BeginPlay()
{
    Super::BeginPlay();

    if (TriggerMeshLeft)
    {
        TriggerMeshLeft->SetRelativeLocation(PointLinks[0].Left);
        TriggerMeshLeft->OnComponentBeginOverlap.AddDynamic(this, &AUSNavLinkProxy::OnTriggerOverlap);
    }

    if (TriggerMeshRight)
    {
        TriggerMeshRight->SetRelativeLocation(PointLinks[0].Right);
        TriggerMeshRight->OnComponentBeginOverlap.AddDynamic(this, &AUSNavLinkProxy::OnTriggerOverlap);
    }

    SetActorHiddenInGame(false);
    GetWorldTimerManager().SetTimerForNextTick([this]()
        {
            // 액터 숨기되 Collision은 유지
            //SetActorHiddenInGame(true);
            SetActorEnableCollision(true);  // Collision 강제 활성화

            // 컴포넌트별로도 명시적 설정
            if (TriggerMeshLeft)
            {
                TriggerMeshLeft->SetVisibility(true, false);
                TriggerMeshLeft->bHiddenInGame = false;
                TriggerMeshLeft->MarkRenderStateDirty();
                TriggerMeshLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
                TriggerMeshLeft->SetCollisionResponseToAllChannels(ECR_Ignore);
                TriggerMeshLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
                TriggerMeshLeft->SetGenerateOverlapEvents(true);
            }

            if (TriggerMeshRight)
            {
                TriggerMeshRight->SetVisibility(true, false);
                TriggerMeshRight->bHiddenInGame = false;
                TriggerMeshRight->MarkRenderStateDirty();
                TriggerMeshRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
                TriggerMeshRight->SetCollisionResponseToAllChannels(ECR_Ignore);
                TriggerMeshRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
                TriggerMeshRight->SetGenerateOverlapEvents(true);
            }
        });
}

void AUSNavLinkProxy::OnLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
    ACharacter* Character = Cast<ACharacter>(MovingActor);
    if (Character == nullptr)
        return;

    switch (ActionType)
    {
    case ENavLinkActionType::Vault: HandleVault(Character, DestinationPoint); break;
    case ENavLinkActionType::Jump:  HandleJump(Character, DestinationPoint);  break;
    case ENavLinkActionType::Climb: HandleClimb(Character, DestinationPoint); break;
    case ENavLinkActionType::Drop:  HandleDrop(Character, DestinationPoint);  break;
    }
}

void AUSNavLinkProxy::HandleJump(ACharacter* Character, const FVector& Destination)
{
    SaveMovementMode = Character->GetCharacterMovement()->MovementMode;
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);

    // 기존 velocity 초기화
    //Character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
    //Character->GetCharacterMovement()->StopMovementImmediately();

    Character->SetActorRotation(JumpConfig.TargetRotation);

    FVector FinalVelocity = JumpConfig.LaunchVelocity;

    if (JumpConfig.bUseLocalDirection)
    {
        // 링크 액터의 로컬 방향 기준으로 변환
        // X = 링크 Forward, Y = Right, Z = Up
        FinalVelocity = GetActorTransform().TransformVector(JumpConfig.LaunchVelocity);
    }

    Character->LaunchCharacter(FinalVelocity, JumpConfig.bOverrideXY, JumpConfig.bOverrideZ);

    FTimerHandle TimerHandle;
    FVector CapturedDest = Destination;
    GetWorldTimerManager().SetTimer(TimerHandle, [this, Character, CapturedDest]()
        {
            ResumeMovement(Character, CapturedDest);
        }, 2.0f, false);
}

void AUSNavLinkProxy::HandleDrop(ACharacter* Character, const FVector& Destination)
{
    AAIController* AIC = Cast<AAIController>(Character->GetController());
    if (AIC)
        AIC->PauseMove(AIC->GetCurrentMoveRequestID());

    FVector DropDir = (Destination - Character->GetActorLocation()).GetSafeNormal2D();
    Character->LaunchCharacter(DropDir * 300.f, true, false);

    FTimerHandle TimerHandle;
    FVector CapturedDest = Destination;
    GetWorldTimerManager().SetTimer(TimerHandle, [this, Character, CapturedDest]()
        {
            ResumeMovement(Character, CapturedDest);
        }, 1.0f, false);
}

void AUSNavLinkProxy::HandleVault(ACharacter* Character, const FVector& Destination)
{
    AAIController* AIC = Cast<AAIController>(Character->GetController());
    if (AIC)
        AIC->PauseMove(AIC->GetCurrentMoveRequestID());

    FTimerHandle TimerHandle;
    FVector CapturedDest = Destination; 
    ACharacter* CapturedCharacter = Character;  // 캡처용 변수로 분리

    GetWorldTimerManager().SetTimer(TimerHandle, [this, Character, CapturedDest, CapturedCharacter]()
        {
            if (CapturedCharacter)  // 유효성 체크 추가
            {
                CapturedCharacter->SetActorLocation(CapturedDest, false, nullptr, ETeleportType::TeleportPhysics);
            }
            ResumeMovement(Character, CapturedDest);
        }, 0.6f, false);
}

void AUSNavLinkProxy::HandleClimb(ACharacter* Character, const FVector& Destination)
{
    AAIController* AIC = Cast<AAIController>(Character->GetController());
    if (AIC)
        AIC->PauseMove(AIC->GetCurrentMoveRequestID());

    FTimerHandle TimerHandle;
    FVector CapturedDest = Destination;
    ACharacter* CapturedCharacter = Character;
    FVector WallForward = GetActorForwardVector(); 
    CapturedDest += WallForward * 120.f;
    CapturedDest.Z += 20.f;

    GetWorldTimerManager().SetTimer(TimerHandle, [this, Character, CapturedDest, CapturedCharacter]()
        {
            if (CapturedCharacter)  // 유효성 체크 추가
            {
                CapturedCharacter->SetActorLocation(CapturedDest, false, nullptr, ETeleportType::TeleportPhysics);
            }
            ResumeMovement(Character, CapturedDest);
        }, 0.8f, false);
}

void AUSNavLinkProxy::ResumeMovement(class ACharacter* Character, const FVector& Destination)
{
    auto OwnerChracterBase = Cast<AUSCharacterBase>(Character);
    if(OwnerChracterBase->IsClimbing())
        return;
    Character->GetCharacterMovement()->SetMovementMode(SaveMovementMode);
}

void AUSNavLinkProxy::DrawDebugLink()
{
    if (PointLinks.Num() == 0)
        return;

    FVector Center = GetActorLocation();
    FVector Start = Center + PointLinks[0].Left;
    FVector End = Center + PointLinks[0].Right;

    FColor LinkColor;
    switch (ActionType)
    {
    case ENavLinkActionType::Jump:  LinkColor = FColor::Yellow; break;
    case ENavLinkActionType::Vault: LinkColor = FColor::Green;  break;
    case ENavLinkActionType::Climb: LinkColor = FColor::Blue;   break;
    case ENavLinkActionType::Drop:  LinkColor = FColor::Red;    break;
    default:                        LinkColor = FColor::White;  break;
    }

    DrawDebugDirectionalArrow(
        GetWorld(),
        Start, End,
        40.f,       // 화살표 크기
        LinkColor,
        true,       // 영구 표시
        -1.f,
        0,
        5.f         // 선 두께
    );

    // 시작/끝 구체
    DrawDebugSphere(GetWorld(), Start, 20.f, 8, FColor::White, true);
    DrawDebugSphere(GetWorld(), End, 20.f, 8, LinkColor, true);

    // 이름 텍스트
    DrawDebugString(GetWorld(), Center + FVector(0, 0, 50), GetActorLabel(), nullptr, LinkColor, -1.f, true);
}

void AUSNavLinkProxy::SetupSmartLink(const FVector& Start, const FVector& End)
{
    PointLinks.Empty();

    SetSmartLinkEnabled(true);

    UNavLinkCustomComponent* SmartComp = GetSmartLinkComp();
    if (SmartComp)
    {
        SmartComp->SetLinkData(Start, End, ENavLinkDirection::LeftToRight);
        SmartComp->RefreshNavigationModifiers();
    }

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSys)
    {
        NavSys->UpdateActorInNavOctree(*this);
        NavSys->Build();  // 에디터에서만, 런타임은 아래 방법
    }
}

void AUSNavLinkProxy::OnTriggerOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (Character == nullptr)
        return;

    FVector Destination;
    FVector TriggerToDestination;

    if (OverlappedComp == TriggerMeshLeft)
    {
        Destination = TriggerMeshRight->GetComponentLocation();
        TriggerToDestination = Destination - TriggerMeshLeft->GetComponentLocation();
    }
    else if (OverlappedComp == TriggerMeshRight)
    {
        Destination = TriggerMeshLeft->GetComponentLocation();
        TriggerToDestination = Destination - TriggerMeshRight->GetComponentLocation();
    }
    else
    {
        return;
    }

    // 캐릭터의 이동 방향 확인
    UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
    if (MovementComp)
    {
        FVector Velocity = MovementComp->Velocity;
        FVector MovementDir = Velocity.GetSafeNormal2D();
        FVector LinkDir = TriggerToDestination.GetSafeNormal2D();

        // 같은 방향인지 체크
        float DotProduct = FVector::DotProduct(MovementDir, LinkDir);

        //if (abs(DotProduct) < 0.1)  // 70도 정도까진 봐주고 그 각이 넘어가면 작동무시, 왼쪽<->오른쪽 왔다갔다하는 문제
        //{
        //    return;
        //}

        if (Velocity.Size2D() < 100.0f)
        {
            return;
        }
    }

    switch (ActionType)
    {
    case ENavLinkActionType::Vault: HandleVault(Character, Destination); break;
    case ENavLinkActionType::Jump:  HandleJump(Character, Destination);  break;
    case ENavLinkActionType::Climb: HandleClimb(Character, Destination); break;
    case ENavLinkActionType::Drop:  HandleDrop(Character, Destination);  break;
    }
}

#if WITH_EDITOR
void AUSNavLinkProxy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    UpdateLaunchArrow();
}

void AUSNavLinkProxy::UpdateLaunchArrow()
{
    if (!LaunchArrow) return;

    FVector Velocity = JumpConfig.LaunchVelocity;

    if (JumpConfig.bUseLocalDirection)
    {
        // 로컬 기준이면 그냥 방향만 사용 (이미 로컬 공간)
        // 화살표 자체가 액터에 붙어있으므로 로컬 방향 그대로 적용
    }
    else
    {
        // 월드 기준이면 역변환해서 로컬 방향으로
        Velocity = GetActorTransform().InverseTransformVector(JumpConfig.LaunchVelocity);
    }

    if (!Velocity.IsNearlyZero())
    {
        // 화살표를 LaunchVelocity 방향으로 회전
        LaunchArrow->SetRelativeRotation(Velocity.Rotation());

        // 속도 크기에 비례해서 화살표 길이 조절 (선택)
        float Speed = Velocity.Size();
        LaunchArrow->SetRelativeScale3D(FVector(Speed / 300.f, 1.f, 1.f));
    }
}
#endif