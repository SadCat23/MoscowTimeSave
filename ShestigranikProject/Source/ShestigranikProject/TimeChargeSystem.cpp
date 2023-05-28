// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeChargeSystem.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(TimeChargeSystemLog,All,All);

void UTimeChargeSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}


int32 UTimeChargeSystem::GetCharges() const
{
    return CurrentCharges;
}

void UTimeChargeSystem::SetMaxCharges(int32 InMaxCharges)
{
    if(InMaxCharges<0) return;
    MaxCharges = InMaxCharges;
    CurrentCharges = FMath::Clamp(CurrentCharges,0,MaxCharges);
    OnChargeUpdate.Broadcast(MaxCharges,CurrentCharges);
}

void UTimeChargeSystem::ResetChargesToMax()
{
    CurrentCharges = MaxCharges;
    OnChargeUpdate.Broadcast(MaxCharges,CurrentCharges);
}

bool UTimeChargeSystem::TrySpendCharges(int32 Charges)
{
    if(Charges<0) return false; 
    if(CurrentCharges<Charges)
    {
        UE_LOG(TimeChargeSystemLog,Display, TEXT("Can't using %d charge, currentCharges %d"), Charges, CurrentCharges);
        return false;
    }
    CurrentCharges = CurrentCharges - Charges;
    OnChargeUpdate.Broadcast(MaxCharges,CurrentCharges);
    UE_LOG(TimeChargeSystemLog,Display, TEXT("Success used %d charge, currentCharges %d"), Charges, CurrentCharges);
    return true;
}

bool UTimeChargeSystem::CheckAvailableCastTimeAbility(int32 AbilityCost)
{
    if(AbilityCost<0) return false; 
    if(CurrentCharges<AbilityCost)
    {
        UE_LOG(TimeChargeSystemLog,Display, TEXT("Can't using %d charge, currentCharges %d"), AbilityCost, CurrentCharges);
        return false;
    }
    UE_LOG(TimeChargeSystemLog,Display, TEXT("Can using %d charge, currentCharges %d"), AbilityCost, CurrentCharges);
    return true;
}
