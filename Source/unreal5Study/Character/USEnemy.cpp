// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USEnemy.h"

AUSEnemy::AUSEnemy()
{
}

bool AUSEnemy::CanBeTargeted() const
{
	return true;
}

FVector AUSEnemy::GetTargetLocation() const
{
	return GetActorLocation();
}
