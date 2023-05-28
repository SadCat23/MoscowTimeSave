// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Engine/DataAsset.h"
#include "DACharacterData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SHESTIGRANIKPROJECT_API UDACharacterData : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    TObjectPtr<USkeletalMesh> Mesh;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    TMap<EAbilitySlot,UDAAbilityData*> Abilities;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    UDAAbilityData* PassiveAbility;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FText Name;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FText Description;
    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    UTexture2D* Icon;
    

};
