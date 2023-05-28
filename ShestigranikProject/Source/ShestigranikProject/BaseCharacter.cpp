// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "DAAbilityData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"
#include "TimeChargeSystem.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ABaseCharacter::ABaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArmComponent->SetupAttachment(GetMesh());
    SpringArmComponent->SetAbsolute(false, true, false);
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraRotateTimeline = CreateDefaultSubobject<UTimelineComponent>("TimelineCameraAnimation");
}

void ABaseCharacter::CrouchTriggered()
{
    if (!IsCrouch())
    {
        StartCrouching();
    }
    else
    {
        StopCrouching();
    }
}

void ABaseCharacter::SetWalkSpeed(const float NewWalkSpeed) const
{
    GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(NewWalkSpeed, 0, RunningSpeed);
}


void ABaseCharacter::StartCrouching()
{
    if (!CanCrouch()) return;
    if (IsRunning()) return;
    Crouch();
}


void ABaseCharacter::StartRunning()
{
    Running = true;
    if (IsCrouch()) UnCrouch();
    SetWalkSpeed(RunningSpeed);
}

void ABaseCharacter::StopCrouching()
{
    if (!IsCrouch()) return;
    UnCrouch();
    SetWalkSpeed(WalkingSpeed);
}

void ABaseCharacter::StopRunning()
{
    if (!IsRunning()) return;
    Running = false;
    SetWalkSpeed(WalkingSpeed);
}

bool ABaseCharacter::IsCrouch() const
{
    return GetCharacterMovement()->IsCrouching();
}

bool ABaseCharacter::IsRunning() const
{
    return Running;
}

void ABaseCharacter::UpdateMovementSpeeds(float InWalkingSpeed, float InCrouchSpeed, float InRunningSpeed)
{
    WalkingSpeed = InWalkingSpeed;
    CrouchingSpeed = InCrouchSpeed;
    RunningSpeed = InRunningSpeed;
    if (IsCrouch())SetWalkSpeed(CrouchingSpeed);
    else if (IsRunning()) SetWalkSpeed(RunningSpeed);
    else SetWalkSpeed(CrouchingSpeed);
}

void ABaseCharacter::RotateCamera(const FInputActionValue& InputValue)
{
    if (CameraRotateTimeline->IsPlaying())
    {
        if (InputValue.GetMagnitude() > 0 && FinisCameraRotateAngle > StartCameraRotateAngle)
        {
            return;
        }
        if (InputValue.GetMagnitude() < 0 && FinisCameraRotateAngle < StartCameraRotateAngle)
        {
            return;
        }
    }
    StartCameraRotateAngle = SpringArmComponent->GetComponentRotation().Yaw;
    if (StartCameraRotateAngle < 0) StartCameraRotateAngle = StartCameraRotateAngle + 360;
    if (StartCameraRotateAngle > 360) StartCameraRotateAngle = StartCameraRotateAngle - 360;
    float TargetAngle = FinisCameraRotateAngle + CameraRotateStep * InputValue.GetMagnitude();
    if (TargetAngle < 0.0f)
    {
        TargetAngle = TargetAngle + 360.0f;
        StartCameraRotateAngle = StartCameraRotateAngle + 360;
    }
    if (TargetAngle > 360.0f) TargetAngle = TargetAngle - 360.0f;
    FinisCameraRotateAngle = TargetAngle;
    PlayCameraTimeline();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABaseCharacter::CameraTimelineUpdate(const float Value)
{
    const float Angle = FMath::Lerp(StartCameraRotateAngle, FinisCameraRotateAngle, Value);
    UE_LOG(LogTemp, Display, TEXT("angle camera %f"), Angle);
    SpringArmComponent->SetWorldRotation(FRotator(DefaultCameraRotation.Pitch, Angle, DefaultCameraRotation.Roll));
}

void ABaseCharacter::CameraTimelineFinishedCallback()
{
}

void ABaseCharacter::PlayCameraTimeline() const
{
    if (!CameraRotateTimeline || !CameraRotateTimeFloatCurve) return;
    CameraRotateTimeline->PlayFromStart();
}

void ABaseCharacter::InitCameraTimeLine()
{
    FinisCameraRotateAngle = SpringArmComponent->GetComponentRotation().Yaw;
    DefaultCameraRotation = SpringArmComponent->GetComponentRotation();
    if (CameraRotateTimeFloatCurve)
    {
        FOnTimelineEventStatic OnTimelineFinishedCallback;
        FOnTimelineFloat OnTimelineCallback;
        CameraRotateTimeline->SetLooping(false);
        CameraRotateTimeline->SetTimelineLength(1.0f);
        OnTimelineCallback.BindDynamic(this, &ABaseCharacter::CameraTimelineUpdate);
        OnTimelineFinishedCallback.BindUObject(this, &ABaseCharacter::CameraTimelineFinishedCallback);
        CameraRotateTimeline->AddInterpFloat(CameraRotateTimeFloatCurve, OnTimelineCallback);
        CameraRotateTimeline->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
    }
}

void ABaseCharacter::InitAbilities()
{
}

bool ABaseCharacter::TryActivateAbility(EAbilitySlot Slot, EAbilityFailedCastReason& ReasonFailed)
{
    if (!Abilities.Contains(Slot)) return false;
    if (GetAbilitiesCooldownState(Slot))
    {
        ReasonFailed = EAbilityFailedCastReason::Cooldown;
        return false;
    }
    const UDAAbilityData* AbilityData = Abilities[Slot];
    const TSubclassOf<UBaseTimeAbility> Ability = Abilities[Slot]->AbilityClass;

    if (!IsValid(Ability)) return false;
    if (ActiveAbility)
    {
        if (ActiveAbility->GetClass() == Ability)
        {
            ActiveAbility->CancelAbility();
            return false;
        }
        ActiveAbility->CancelAbility();
    }
    ActiveAbility = NewObject<UBaseTimeAbility>(this, Ability);
    ActiveAbility->OnAbilityCancel.AddDynamic(this, &ABaseCharacter::OnCancelActiveAbility);
    ActiveAbility->OnAbilityFinish.AddDynamic(this, &ABaseCharacter::OnFinishAbility);
    const bool SuccessActivate = ActiveAbility->TryActivate(AbilityData->Cost);
    if (!SuccessActivate)
    {
        ReasonFailed = EAbilityFailedCastReason::NotEnoughCharges;
        ActiveAbility->CancelAbility();
        return false;
    }

    UCooldownEffect* CooldownEffect;
    if (AbilityData->OneCharge)
    {
        CooldownEffect = UCooldownEffect::GetPermanentCooldownObject(ActiveAbility);
    }
    else
    {
        CooldownEffect = UCooldownEffect::GetCooldownObjectByTime(ActiveAbility, AbilityData->Cooldown);
        CooldownEffect->OnFinishCooldown.BindUObject(this, &ABaseCharacter::FinishCooldown);
    }
    CooldownMap.Add(ActiveAbility->GetClass(), CooldownEffect);
    ExecutingAbilities.Add(ActiveAbility);
    OnSetActiveAbility.Broadcast(Slot, true);
    return true;
}

bool ABaseCharacter::GetCooldownInfoBySlot(EAbilitySlot Slot, int32& RemainingTime, int32& FullTime, bool& IsOneCharge)
{
    RemainingTime = 0;
    FullTime = 0;
    if (Abilities.Contains(Slot))
    {
        if (CooldownMap.Contains(Abilities[Slot]->AbilityClass))
        {
            if (Abilities[Slot]->OneCharge)
            {
                IsOneCharge = true;
                return true;
            }
            const UCooldownEffect* Effect = CooldownMap[Abilities[Slot]->AbilityClass];
            RemainingTime = Effect->GetRemainingCooldownTime();
            FullTime = Effect->GetFullCooldownDuration();
            return true;
        }
    }
    return false;
}

bool ABaseCharacter::GetAbilitiesCooldownState(EAbilitySlot Slot)
{
    if (Abilities.Contains(Slot))
    {
        return CooldownMap.Contains(Abilities[Slot]->AbilityClass);
    }

    return false;
}

void ABaseCharacter::SetAbilityToSlot(EAbilitySlot Slot, UDAAbilityData* AbilityClass)
{
    Abilities.Add(Slot, AbilityClass);
}

void ABaseCharacter::OnFinishAbility(UBaseTimeAbility* Ability)
{
    Ability->ConditionalBeginDestroy();
    ExecutingAbilities.Remove(Ability);
}

void ABaseCharacter::OnCancelActiveAbility(UBaseTimeAbility* Ability)
{
    if (ActiveAbility == Ability)
    {
        ResetActiveAbility();
    }
    
}

void ABaseCharacter::ResetActiveAbility()
{
    ActiveAbility->ConditionalBeginDestroy();
    OnSetActiveAbility.Broadcast(EAbilitySlot::None, false);
    ExecutingAbilities.Remove(ActiveAbility);
    ActiveAbility = nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABaseCharacter::CommitAbility()
{
    if (!ActiveAbility) return;
    ActiveAbility->CommitAbility();
}

void ABaseCharacter::FinishCooldown(UCooldownEffect* Effect)
{
    const UBaseTimeAbility* Ability = Cast<UBaseTimeAbility>(Effect->GetOuter());
    CooldownMap.Remove(Ability->GetClass());
}

void ABaseCharacter::SetAbilitiesSet(TMap<EAbilitySlot, UDAAbilityData*> InAbilities)
{
    Abilities = InAbilities;
}

void ABaseCharacter::StopCooldowns()
{
    for (auto ColdownEffect : CooldownMap)
    {
        ColdownEffect.Value->Stop();
    }
}

void ABaseCharacter::StartCooldowns()
{
    for (auto ColdownEffect : CooldownMap)
    {
        ColdownEffect.Value->Start();
    }
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    SetWalkSpeed(WalkingSpeed);
    GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchingSpeed;
    InitCameraTimeLine();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //GetPlayerCtrl
    const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
    check(PlayerController);

    //GetInputSystem
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
        PlayerController->GetLocalPlayer());

    //Add Input Mapping to Input System
    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMapping, 0);

    //Get InputComponent
    UEnhancedInputComponent* IComponent = Cast<UEnhancedInputComponent>(InputComponent);

    //Bind movement input actions
    IComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

    IComponent->BindAction(CrouchingInputAction, ETriggerEvent::Triggered, this, &ThisClass::CrouchTriggered);

    IComponent->BindAction(RunningInputAction, ETriggerEvent::Started, this, &ThisClass::StartRunning);
    IComponent->BindAction(RunningInputAction, ETriggerEvent::Completed, this, &ThisClass::StopRunning);

    IComponent->BindAction(RotateCameraInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);

    //Ability
    IComponent->BindAction(CommitAbilityInputAction, ETriggerEvent::Triggered, this, &ThisClass::CommitAbility);
}

void ABaseCharacter::Move(const FInputActionValue& InputValue)
{
    const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return;

    const FVector2D Value = InputValue.Get<FVector2D>();
    const FRotator MovementRotation(0.0f, SpringArmComponent->GetComponentRotation().Yaw, 0.0f);

    if (Value.X != 0.0f)
    {
        const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
        AddMovementInput(MovementDirection, Value.X);
    }
    if (Value.Y != 0.0f)
    {
        const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(MovementDirection, Value.Y);
    }
}
