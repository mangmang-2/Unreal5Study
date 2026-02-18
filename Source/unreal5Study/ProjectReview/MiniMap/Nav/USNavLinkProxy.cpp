// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MiniMap/Nav/USNavLinkProxy.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavLinkCustomComponent.h"
#include "NavigationSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUSNavLinkProxy::AUSNavLinkProxy()
{
    // SmartLink 활성화 (OnSmartLinkReached 이벤트 발생 조건)
    SetSmartLinkEnabled(true);

    // NewObject 대신 CreateDefaultSubobject 사용
    TriggerMeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMeshLeft"));
    TriggerMeshLeft->SetupAttachment(RootComponent);

    TriggerMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMeshRight"));
    TriggerMeshRight->SetupAttachment(RootComponent);
}

void AUSNavLinkProxy::BeginPlay()
{
    Super::BeginPlay();

    TriggerMeshLeft->SetRelativeLocation(PointLinks[0].Left);
    TriggerMeshLeft->OnComponentBeginOverlap.AddDynamic(this, &AUSNavLinkProxy::OnTriggerOverlap);

    TriggerMeshRight->SetRelativeLocation(PointLinks[0].Right);
    TriggerMeshRight->OnComponentBeginOverlap.AddDynamic(this, &AUSNavLinkProxy::OnTriggerOverlap);
    SetActorHiddenInGame(false);
    GetWorldTimerManager().SetTimerForNextTick([this]()
        {
            // 액터 숨기되 Collision은 유지
            //SetActorHiddenInGame(true);
            SetActorEnableCollision(true);  // Collision 강제 활성화

            // 컴포넌트별로도 명시적 설정
            TriggerMeshLeft->SetVisibility(true, false);
            TriggerMeshLeft->bHiddenInGame = false;
            TriggerMeshLeft->MarkRenderStateDirty();
			TriggerMeshLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			TriggerMeshLeft->SetCollisionResponseToAllChannels(ECR_Ignore);
			TriggerMeshLeft->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			TriggerMeshLeft->SetGenerateOverlapEvents(true);

            TriggerMeshRight->SetVisibility(true, false);
            TriggerMeshRight->bHiddenInGame = false;
            TriggerMeshRight->MarkRenderStateDirty();
            TriggerMeshRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            TriggerMeshRight->SetCollisionResponseToAllChannels(ECR_Ignore);
            TriggerMeshRight->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
            TriggerMeshRight->SetGenerateOverlapEvents(true);
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
    AAIController* AIC = Cast<AAIController>(Character->GetController());
    if (AIC)
        AIC->StopMovement();

    FVector Start = Character->GetActorLocation();
    FVector Delta = Destination - Start;

    // LaunchVelocity 계산
    float HorizontalDist = Delta.Size2D();
    float HeightDiff = Delta.Z;
    float Gravity = FMath::Abs(GetWorld()->GetGravityZ());
    float LaunchAngle = FMath::DegreesToRadians(45.0f);

    float Velocity = FMath::Sqrt((Gravity * HorizontalDist) / FMath::Sin(2.0f * LaunchAngle));
    FVector HorizontalDir = Delta.GetSafeNormal2D();

    FVector LaunchVelocity;
    LaunchVelocity.X = HorizontalDir.X * Velocity * FMath::Cos(LaunchAngle);
    LaunchVelocity.Y = HorizontalDir.Y * Velocity * FMath::Cos(LaunchAngle);
    LaunchVelocity.Z = Velocity * FMath::Sin(LaunchAngle);

    if (FMath::Abs(HeightDiff) > 10.0f)
    {
        float TimeToReach = HorizontalDist / (Velocity * FMath::Cos(LaunchAngle));
        LaunchVelocity.Z = (HeightDiff + 0.5f * Gravity * TimeToReach * TimeToReach) / TimeToReach;
    }
    LaunchVelocity *= 1.3;
    Character->LaunchCharacter(LaunchVelocity, true, true);

    FTimerHandle InputTimerHandle;
    FVector CapturedDir = HorizontalDir;

    GetWorldTimerManager().SetTimer(InputTimerHandle,
        [this, Character, CapturedDir]()
        {
            if (Character && Character->GetCharacterMovement() &&
                Character->GetCharacterMovement()->IsFalling())
            {
                // 공중에 있는 동안 앞으로 계속 입력을 줌
                Character->AddMovementInput(CapturedDir, 1.0f);
            }
        },
        0.02f,
        true 
    );

    // ★ 착지 후 타이머 정리 및 이동 재개
    FTimerHandle LandingTimerHandle;
    FVector CapturedDest = Destination;

    GetWorldTimerManager().SetTimer(LandingTimerHandle,
        [this, AIC, Character, CapturedDest, InputTimerHandle]() mutable  // ★ mutable 추가
        {
            GetWorldTimerManager().ClearTimer(InputTimerHandle);

            ResumeMovement(AIC, CapturedDest); // 다시 움직임

        }, 1.5f, false);
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
    GetWorldTimerManager().SetTimer(TimerHandle, [this, AIC, CapturedDest]()
        {
            ResumeMovement(AIC, CapturedDest);
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

    GetWorldTimerManager().SetTimer(TimerHandle, [this, AIC, CapturedDest, CapturedCharacter]()
        {
            if (CapturedCharacter)  // 유효성 체크 추가
            {
                CapturedCharacter->SetActorLocation(CapturedDest, false, nullptr, ETeleportType::TeleportPhysics);
            }
            ResumeMovement(AIC, CapturedDest);
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

    GetWorldTimerManager().SetTimer(TimerHandle, [this, AIC, CapturedDest, CapturedCharacter]()
        {
            if (CapturedCharacter)  // 유효성 체크 추가
            {
                CapturedCharacter->SetActorLocation(CapturedDest, false, nullptr, ETeleportType::TeleportPhysics);
            }
            ResumeMovement(AIC, CapturedDest);
        }, 0.8f, false);
}

void AUSNavLinkProxy::ResumeMovement(AAIController* AIC, const FVector& Destination)
{
    if (AIC == nullptr)
        return;

    // 일시정지 해제 후 남은 경로 재개
    AIC->ResumeMove(AIC->GetCurrentMoveRequestID());
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

        if (DotProduct < 0.2)  // 70도 정도까진 봐주고 그 각이 넘어가면 작동무시, 왼쪽<->오른쪽 왔다갔다하는 문제
        {
            return;
        }

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