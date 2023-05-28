// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIHelperLib.generated.h"

/**
 * 
 */
UCLASS()
class SHESTIGRANIKPROJECT_API UAIHelperLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    static void SetAISightPerception(UAIPerceptionComponent* Comp,float MaxRadius, float AngleView);
};
