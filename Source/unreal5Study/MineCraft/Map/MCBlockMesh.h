// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "MCBlockMesh.generated.h"

/**
 * 추후에 블록 종류로 머터리얼을 설정 할것
 * 지금은 단일 매쉬로 모양만 잡음
 */
UCLASS()
class UNREAL5STUDY_API UMCBlockMesh : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
	
};
