// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Ability/USCropout_SimpleAttackCheck.h"
#include "../../Character/Ability/AT/USAT_AttackTrace.h"
#include "../Stat/USCropoutStat.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UUSCropout_SimpleAttackCheck::UUSCropout_SimpleAttackCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUSCropout_SimpleAttackCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* OwnerActor = Cast<AActor>(ActorInfo->AvatarActor.Get());
    if (OwnerActor == nullptr)
        return;

    UWorld* World = OwnerActor->GetWorld();
    if (World == nullptr)
        return;

    if(GameplayEffectClass == nullptr)
        return;

    FVector ForwardVector = OwnerActor->GetActorForwardVector();
    FVector Start = OwnerActor->GetActorLocation() + ForwardVector * AttackRange;
    
    FVector End = Start + (ForwardVector * AttackRange);

    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AttackCheck), false, OwnerActor);

    FHitResult HitResult;
    bool bHit = World->SweepSingleByChannel(
        HitResult,
        Start,
        End,
        FQuat::Identity, 
        ECC_Visibility,
        FCollisionShape::MakeSphere(AttackRange),
        TraceParams
    );

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {       
            UAbilitySystemComponent* HitASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
            if (HitASC)
            {
                FGameplayEffectContextHandle EffectContext = HitASC->MakeEffectContext();
                EffectContext.AddSourceObject(ActorInfo->AvatarActor.Get());
                FGameplayEffectSpecHandle EffectSpecHandle = HitASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
                if (EffectSpecHandle.IsValid())
                {
                    HitASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
                }
            }

            FVector Center = (Start + End) * 0.5f;
            FColor LineColor = bHit ? FColor::Red : FColor::Green;
            DrawDebugSphere(
                World,
                Center,
                AttackRange,
                12,
                FColor::Red,
                false,
                2.0f
            );
        }
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UUSCropout_SimpleAttackCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
