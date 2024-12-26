// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USCropoutMonster.h"
#include "Kismet/GameplayStatics.h"

void AUSCropoutMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float AccTime = 0.0f;
	AccTime += DeltaTime;

	if (AccTime <= 3.0f)
		return;

	AccTime = 0;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), FoundActors);
	for (const auto& Actor : FoundActors)
	{
		if (Actor == nullptr)
			continue;

		float DistToPawn = FVector::Distance(Actor->GetActorLocation(), GetActorLocation());

		if (DistToPawn < 300)
		{
			Tags[0] = TEXT("");
			ChangeJob(TEXT("Monster"));
			Tags[0] = TEXT("Attack");
			break;
		}
	}
}

void AUSCropoutMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ChangeJob(TEXT("Monster"));
	Tags[0] = TEXT("Attack");
	Tags.AddUnique("Monster");
	
}

void AUSCropoutMonster::PlayWorkAnim_Implementation(float Delay)
{
	PlayVillagerAnim(WorkAnim, Delay);

	if (Tool && TargetTool)
	{
		Tool->SetStaticMesh(TargetTool);
	}
}
