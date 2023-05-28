// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CooldownEffect.generated.h"

class UCooldownEffect;
DECLARE_DELEGATE_OneParam(FOnFinishCooldown, UCooldownEffect*);
/**
 * 
 */
UCLASS()
class SHESTIGRANIKPROJECT_API UCooldownEffect : public UObject
{
    GENERATED_BODY()

public:
    
    static UCooldownEffect* GetCooldownObjectByTime(UObject* Outer,int32 CooldownDurationInSec);
    static UCooldownEffect* GetPermanentCooldownObject(UObject* Outer);

    void Stop();
    void Start();
    
    int32 GetRemainingCooldownTime() const;
    int32 GetFullCooldownDuration() const;
    FOnFinishCooldown OnFinishCooldown;
private:
    FTimerHandle CooldownTimer;
    int32 CooldownDuration;
    void InitCooldownTimer();
    void FinishCooldown();
};
