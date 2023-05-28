// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownEffect.h"


UCooldownEffect* UCooldownEffect::GetCooldownObjectByTime(UObject* Outer, int32 CooldownDurationInSec)
{
    if (!Outer) return nullptr;
    if (!Outer->GetWorld()) return nullptr;
    UCooldownEffect* CooldownEffect = NewObject<UCooldownEffect>(Outer);
    CooldownEffect->CooldownDuration = CooldownDurationInSec;
    CooldownEffect->InitCooldownTimer();
    return CooldownEffect;
}

UCooldownEffect* UCooldownEffect::GetPermanentCooldownObject(UObject* Outer)
{
    if (!Outer) return nullptr;
    if (!Outer->GetWorld()) return nullptr;
    UCooldownEffect* CooldownEffect = NewObject<UCooldownEffect>(Outer);
    return CooldownEffect;
}

void UCooldownEffect::Stop()
{
    if (CooldownTimer.IsValid())
    {
        GetOuter()->GetWorld()->GetTimerManager().PauseTimer(CooldownTimer);
    }
}

void UCooldownEffect::Start()
{
    if (CooldownTimer.IsValid())
    {
        GetOuter()->GetWorld()->GetTimerManager().UnPauseTimer(CooldownTimer);
    }
}


int32 UCooldownEffect::GetRemainingCooldownTime() const
{
    if (CooldownTimer.IsValid())
    {
        return GetOuter()->GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer);
    }
    return 0;
}

int32 UCooldownEffect::GetFullCooldownDuration() const
{
    return CooldownDuration;
}

void UCooldownEffect::InitCooldownTimer()
{
    GetOuter()->GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UCooldownEffect::FinishCooldown,
        CooldownDuration, false);
}

void UCooldownEffect::FinishCooldown()
{
    OnFinishCooldown.ExecuteIfBound(this);
}
