// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeChargeSystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChargeUpdateDelegate,int32,MaxCharges,int32,CurrentCharges);

/**
 * 
 */
UCLASS()
class SHESTIGRANIKPROJECT_API UTimeChargeSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    UPROPERTY(BlueprintAssignable)
    FOnChargeUpdateDelegate OnChargeUpdate;
    
    UFUNCTION(BlueprintPure)
    int32 GetCharges() const;

    UFUNCTION(BlueprintCallable)
    void SetMaxCharges(int32 InMaxCharges);

    UFUNCTION(BlueprintCallable)
    void ResetChargesToMax();

    UFUNCTION(BlueprintCallable)
    bool TrySpendCharges(int32 Charges);

    UFUNCTION(BlueprintCallable)
    bool CheckAvailableCastTimeAbility(int32 AbilityCost);
    
private:
    UPROPERTY()
    int32 MaxCharges;
    UPROPERTY()
    int32 CurrentCharges;
};
