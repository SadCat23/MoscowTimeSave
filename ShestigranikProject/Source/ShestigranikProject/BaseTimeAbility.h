// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseTimeAbility.generated.h"

/**
 * 
 */
class UBaseTimeAbility;
class ABaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCancelAbility,UBaseTimeAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinish,UBaseTimeAbility*, Ability);

UCLASS(Abstract,Blueprintable,BlueprintType)
class SHESTIGRANIKPROJECT_API UBaseTimeAbility : public UObject, public FTickableGameObject
{
    GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnStopTime();
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void OnStartTime();

    
    virtual UWorld* GetWorld() const override;
    // Tick interface
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;
            
    UFUNCTION(BlueprintCallable,Category="Ability")
    bool TryActivate(int32 InCost);

    UFUNCTION(BlueprintCallable,Category="Ability")
    bool TryExecute();

    UFUNCTION(BlueprintPure,Category="Ability")
    bool IsCanUse() const;

    UFUNCTION(BlueprintCallable)
    void CancelAbility();
    void CommitAbility();
    
    UFUNCTION(BlueprintCallable)
    void FinishAbility();
    
    UFUNCTION(BlueprintPure)
    FVector GetOwnerLocation();

    UFUNCTION(BlueprintPure)
    ABaseCharacter* GetOwnerCharacter();
    
    UPROPERTY()
    FOnCancelAbility OnAbilityCancel;
    UPROPERTY()
    FOnFinish OnAbilityFinish;
protected:
    UFUNCTION(BlueprintImplementableEvent)
    void EventTickAbility(float DeltaTime);
    UFUNCTION(BlueprintImplementableEvent)
    void OnExecute();
    UFUNCTION(BlueprintImplementableEvent)
    void OnCommitAbility();
    UFUNCTION(BlueprintImplementableEvent)
    void OnActivate();
    UFUNCTION(BlueprintImplementableEvent)
    void OnCancel();

private:
    int32 Cost=0;
};
