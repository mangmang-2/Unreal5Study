// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USCropoutMonster.h"

void AUSCropoutMonster::Action(AActor* VillagerAction)
{
	if (VillagerAction == nullptr)
		return;

	TargetRef = VillagerAction;

	if (TargetRef && TargetRef->IsValidLowLevel())
	{
		if (TargetRef->Tags.IsValidIndex(0))
		{
			FName NewJob = TargetRef->Tags[0];
			ChangeJob(NewJob);
			UpdateAllPawn();
		}
	}
}
