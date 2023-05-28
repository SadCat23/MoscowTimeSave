// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTimeAbility.h"

#include "BaseCharacter.h"
#include "TimeChargeSystem.h"
#include "Kismet/GameplayStatics.h"


UWorld* UBaseTimeAbility::GetWorld() const
{
    if (GIsEditor && !GIsPlayInEditorWorld) return nullptr;
    if (GetOuter()) return GetOuter()->GetWorld();
    return nullptr;
}

void UBaseTimeAbility::Tick(float DeltaTime)
{
    if(IsUnreachable()) return;
    if(!IsValid(this)) return;
    EventTickAbility(DeltaTime);
}

bool UBaseTimeAbility::IsTickable() const
{
    return true;
}

TStatId UBaseTimeAbility::GetStatId() const
{
    return TStatId();
}

bool UBaseTimeAbility::TryActivate(int32 InCost)
{
    Cost = InCost;
    if(!IsCanUse()) return false;
    OnActivate();
    return true;
}

bool UBaseTimeAbility::TryExecute()
{
    if(!IsCanUse()) return false;
    const TObjectPtr<UGameInstance> GI = UGameplayStatics::GetGameInstance(GetOuter());
    const TObjectPtr<UTimeChargeSystem> ChargeSystem = GI->GetSubsystem<UTimeChargeSystem>();
    const bool SuccessSpend = ChargeSystem->TrySpendCharges(Cost);
    OnExecute();
    return SuccessSpend;
}

bool UBaseTimeAbility::IsCanUse() const
{
    const TObjectPtr<UGameInstance> GI = UGameplayStatics::GetGameInstance(GetOuter());
    const TObjectPtr<UTimeChargeSystem> ChargeSystem = GI->GetSubsystem<UTimeChargeSystem>();
    const bool CanUse = ChargeSystem->CheckAvailableCastTimeAbility(Cost);
    return CanUse;
}

void UBaseTimeAbility::CancelAbility()
{
    OnAbilityCancel.Broadcast(this);
    OnCancel();
}

void UBaseTimeAbility::CommitAbility()
{
    OnCommitAbility();
}

void UBaseTimeAbility::FinishAbility()
{
    OnAbilityFinish.Broadcast(this);
}

FVector UBaseTimeAbility::GetOwnerLocation()
{
    const AActor* OwnerActor = Cast<AActor>(GetOuter());
    if(!OwnerActor) return {};
    return OwnerActor->GetActorLocation();
}

ABaseCharacter* UBaseTimeAbility::GetOwnerCharacter()
{
    if (!GetOuter()) return nullptr;
    UObject* Outer = GetOuter();
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Outer);
    return BaseCharacter;
}

