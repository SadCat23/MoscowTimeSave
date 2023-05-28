// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MeshHelper.generated.h"

/**
 * 
 */
UCLASS()
class SHESTIGRANIKPROJECT_API UMeshHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static FVector GetCenter(UStaticMesh* SM);
};
