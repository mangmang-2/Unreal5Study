// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ModularCharacter/USModularCharacterComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UUSModularCharacterComponent::UUSModularCharacterComponent()
{
	Helmet2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet2"));
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));
	Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
	ArmArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmArmor"));
	PantsArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PantsArmor"));
	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	ShoulderArmor = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoulderArmor"));
	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
}


// Called when the game starts
void UUSModularCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	if (owner == nullptr)
		return;
	
	auto Mesh = owner->GetMesh();
	if (Mesh == nullptr)
		return;

	Helmet2->SetLeaderPoseComponent(Mesh);
	Helmet2->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Helmet->SetLeaderPoseComponent(Mesh);
	Helmet->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Gloves->SetLeaderPoseComponent(Mesh);
	Gloves->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Pants->SetLeaderPoseComponent(Mesh);
	Pants->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	ArmArmor->SetLeaderPoseComponent(Mesh);
	ArmArmor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PantsArmor->SetLeaderPoseComponent(Mesh);
	PantsArmor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Shoes->SetLeaderPoseComponent(Mesh);
	Shoes->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	ShoulderArmor->SetLeaderPoseComponent(Mesh);
	ShoulderArmor->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	Torso->SetLeaderPoseComponent(Mesh);
	Torso->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
