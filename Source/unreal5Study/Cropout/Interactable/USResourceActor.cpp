// Fill out your copyright notice in the Description page of Project Settings.


#include "Cropout/Interactable/USResourceActor.h"
#include "USResourceInterface.h"
#include "Kismet/KismetMathLibrary.h"

void AUSResourceActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FString DisplayName = StaticEnum<EResourceType>()->GetDisplayNameTextByIndex(static_cast<int32>(ResourceType)).ToString();
    Tags.Add(FName(DisplayName));

    if (UseRandomMesh)
    {
        int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, MeshList.Num() - 1);
        if (Mesh)
        {
            Mesh->SetStaticMesh(MeshList[RandomIndex]);
        }
    }
}

float AUSResourceActor::Interact()
{
    return CollectionTime;
}

void AUSResourceActor::RemoveResource_Implementation(EResourceType& TargetResouce, int32& Value)
{
    EndWobble();

    if (ResouceAmount == -1.0f)
    {
        TargetResouce = ResourceType;
        Value = CollectionValue;
        return;
    }

    ResouceAmount = FMath::Max(ResouceAmount - CollectionValue, 0.0f);;

    if (ResouceAmount <= 0)
    {
        Death();
        TargetResouce = ResourceType;
        Value = CollectionValue;
        return;
    }

    TargetResouce = ResourceType;
    Value = CollectionValue;
    return;
}

void AUSResourceActor::Death()
{
    Destroy();
}
