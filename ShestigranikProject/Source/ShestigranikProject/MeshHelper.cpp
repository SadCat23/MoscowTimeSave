// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshHelper.h"

FVector UMeshHelper::GetCenter(UStaticMesh* SM)
{
    if(!SM) return {};
    const FBoxSphereBounds MeshBounds = SM->GetBounds();
    const FVector MeshCenter = MeshBounds.GetSphere().Center;
    return MeshCenter;
}
