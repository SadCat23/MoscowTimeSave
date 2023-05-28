// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Engine/DataAsset.h"
#include "DAAbilityData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SHESTIGRANIKPROJECT_API UDAAbilityData : public UDataAsset
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    TSubclassOf<UBaseTimeAbility>  AbilityClass;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    int32 Cost;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    float Cooldown;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    bool OneCharge;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FText Name;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FText Description;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    UTexture2D* Icon;

};
