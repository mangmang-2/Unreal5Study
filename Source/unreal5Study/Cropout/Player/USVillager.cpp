// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Player/USVillager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/DecalComponent.h"

// Sets default values
AUSVillager::AUSVillager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VillagerCapsule"));
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerHat"));
	Hat->SetupAttachment(SkeletalMesh);

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VillagerHair"));
	Hair->SetupAttachment(SkeletalMesh);

	Tool = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VillagerTool"));
	Tool->SetupAttachment(SkeletalMesh, TEXT("hand_rSocket"));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("VillagerMovement"));

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("VillagerDecal"));
	Decal->SetupAttachment(SkeletalMesh);
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

