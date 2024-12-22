// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USVillager.h"


// Sets default values
AUSVillager::AUSVillager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AUSVillager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUSVillager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUSVillager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float AUSVillager::PlayDeliverAnim_Implementation()
{
	PlayVillagerAnim(PutDownAnim, 1.0f);

	return 1.0f;
}

void AUSVillager::PlayWorkAnim_Implementation(float Delay)
{
	PlayVillagerAnim(WorkAnim, Delay);

	if (Tool && TargetTool)
	{
		Tool->SetStaticMesh(TargetTool);
	}

}

void AUSVillager::ReturnToDefaultBT_Implementation()
{
	ChangeJob(FName(TEXT("Idle")));
}


void AUSVillager::AddResource_Implementation(EResourceType Resource, int32 Value)
{
	ResourcesHeld = Resource;
	Quantity = Value;

	if (Tool && CratMesh)
	{
		Tool->SetStaticMesh(CratMesh);
	}
}

void AUSVillager::RemoveResource_Implementation(EResourceType& Resource, int32& Value)
{
	Resource = ResourcesHeld;
	Value = Quantity;

	ResourcesHeld = EResourceType::None;
	Quantity = 0;

	if (Tool)
	{
		Tool->SetStaticMesh(nullptr);
	}
}

