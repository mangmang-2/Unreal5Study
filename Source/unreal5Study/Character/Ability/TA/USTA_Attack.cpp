// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ability/TA/USTA_Attack.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../USCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "../Tag/USGameplayTag.h"

AUSTA_Attack::AUSTA_Attack()
{
}

void AUSTA_Attack::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AUSTA_Attack::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AUSTA_Attack::MakeTargetData() const
{
	AUSCharacterBase* Character = CastChecked<AUSCharacterBase>(SourceActor);
	const UStaticMeshComponent* WeaponMesh = Character->GetWeaponMesh();
	if (WeaponMesh == nullptr)
		return FGameplayAbilityTargetDataHandle();

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(SourceActor); // �� ���ʹ� Ʈ���̽����� ����

	const FVector Start = WeaponMesh->GetComponentLocation();
	const FVector End = WeaponMesh->GetSocketLocation("EndPoint");

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Pawn,
		QueryParams
	);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (bHit)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (ASC == nullptr)
			return FGameplayAbilityTargetDataHandle();

		if (ASC->HasMatchingGameplayTag(USTAG_CHARACTER_INVINSIBLE_ATTACKHIT))
		{
			//DrawDebugLine(
			//	GetWorld(),
			//	Start,
			//	End,
			//	FColor::Red,
			//	false,  // ���������� �׸� ������ ����
			//	1.0f,   // ���� �ð�
			//	0,      // DepthPriority
			//	1.0f    // ���� �β�
			//);
			return FGameplayAbilityTargetDataHandle();
		}

		

		UE_LOG(LogTemp, Warning, TEXT("AUSTA_Attack::MakeTargetData()"));
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);
	}

	FColor DrawColor = bHit ? FColor::Green : FColor::Red;

	//DrawDebugLine(
	//	GetWorld(),
	//	Start,
	//	End,
	//	DrawColor,
	//	false,  // ���������� �׸� ������ ����
	//	1.0f,   // ���� �ð�
	//	0,      // DepthPriority
	//	1.0f    // ���� �β�
	//);

	return DataHandle;
}
