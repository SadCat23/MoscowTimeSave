// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTimeAbility.h"
#include "CooldownEffect.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "BaseCharacter.generated.h"

class UTimelineComponent;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UDAAbilityData;




UENUM(BlueprintType)
enum class EAbilitySlot : uint8
{
    None,
    FirstAbility,
    Ultimate
};

UENUM(BlueprintType)
enum class EAbilityFailedCastReason : uint8
{
    None,
    Cooldown,
    NotEnoughCharges
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetActiveAbility,EAbilitySlot,Slot,bool,SuccessActivate);

UCLASS()
class SHESTIGRANIKPROJECT_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly,VisibleAnywhere,meta=(AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> SpringArmComponent;
    UPROPERTY(BlueprintReadOnly,VisibleAnywhere,meta=(AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> CameraComponent;
    
public:
    // Sets default values for this character's properties
    ABaseCharacter();
   

    //Movement
#pragma region Movement
    /**
     * @brief Speed when character Walking
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MoscowLDTCharacter|Movement")
    float WalkingSpeed;
    
    /**
    * @brief Speed when character Walking
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MoscowLDTCharacter|Movement")
    float CrouchingSpeed;
    
    /**
    * @brief Speed when character Walking
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MoscowLDTCharacter|Movement")
    float RunningSpeed;
    
    /**
     * @brief Start character crouch if it possible (if character dont run)
     */
    UFUNCTION(BlueprintCallable, Category="MoscowLDTCharacter|Movement")
    void StartCrouching();
    
    /**
     * @brief Start character run, stop crouch if character crouching
     */
    UFUNCTION(BlueprintCallable, Category="MoscowLDTCharacter|Movement")
    void StartRunning();
    
    /**
     * @brief Stop crouching if character crouching
     */
    UFUNCTION(BlueprintCallable, Category="MoscowLDTCharacter|Movement")
    void StopCrouching();
    
    /**
     * @brief Stop running if character running
     */
    UFUNCTION(BlueprintCallable, Category="MoscowLDTCharacter|Movement")
    void StopRunning();
    
    /**
     * @brief Check this character are crouching
     * @return return true if character crouching
     */
    UFUNCTION(BlueprintPure, Category="MoscowLDTCharacter|Movement")
    bool IsCrouch() const;
    
    /**
     * @brief Check this character are running
     * @return return true if character running
     */
    UFUNCTION(BlueprintPure, Category="MoscowLDTCharacter|Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category="MoscowLDTCharacter|Movement")
    void UpdateMovementSpeeds(float InWalkingSpeed,float InCrouchSpeed, float InRunningSpeed);
#pragma endregion Movement

#pragma region Input
    /**
     * @brief Input mapping from Enhanced Input System
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input")
    TObjectPtr<UInputMappingContext> InputMapping;
    
    /**
     * @brief Input Action for movement
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Movement")
    TObjectPtr<UInputAction> MoveInputAction;
    
    /**
    * @brief Input Action for running
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Movement")
    TObjectPtr<UInputAction> RunningInputAction;

    /**
    * @brief Input Action for crouching
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Movement")
    TObjectPtr<UInputAction> CrouchingInputAction;

    /**
    * @brief Input Action for rotate camera
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Camera")
    TObjectPtr<UInputAction> RotateCameraInputAction;

    /**
    * @brief Input Action for commit ability
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Ability")
    TObjectPtr<UInputAction> CommitAbilityInputAction;

    /**
    * @brief Input Action for activate ability slot
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Ability")
    TObjectPtr<UInputAction> FirstSlotAbilityInput;
    
    /**
    * @brief Input Action for activate ability slot
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MoscowLDTCharacter|Input|Ability")
    TObjectPtr<UInputAction> UltimateSlotAbilityInput;

#pragma endregion Input

#pragma region Camera
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MoscowLDTCharacter|Camera")
    float CameraRotateStep = 90.0;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MoscowLDTCharacter|Camera")
    float RotateDuration = 1.0;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MoscowLDTCharacter|Camera")
    TObjectPtr<UCurveFloat> CameraRotateTimeFloatCurve;

#pragma endregion Camera

#pragma region Ability
    UPROPERTY(BlueprintAssignable)
    FOnSetActiveAbility OnSetActiveAbility;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="MoscowLDTCharacter|Ability")
    TMap<EAbilitySlot,UDAAbilityData*> Abilities;
    UFUNCTION(BlueprintCallable)
    bool TryActivateAbility(EAbilitySlot Slot, EAbilityFailedCastReason& ReasonFailed);
    UFUNCTION(BlueprintPure)
    bool GetCooldownInfoBySlot(EAbilitySlot Slot, int32& RemainingTime, int32& FullTime, bool& IsOneCharge);
    UFUNCTION(BlueprintCallable)
    bool GetAbilitiesCooldownState(EAbilitySlot Slot);
    UFUNCTION(BlueprintCallable)
    void SetAbilityToSlot(EAbilitySlot Slot, UDAAbilityData* AbilityClass);
    UFUNCTION(BlueprintCallable)
    void SetAbilitiesSet(TMap<EAbilitySlot, UDAAbilityData*> InAbilities);
    UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
    bool bIsAbilityUsed;
    UFUNCTION(BlueprintCallable)
    void StopCooldowns();
    UFUNCTION(BlueprintCallable)
    void StartCooldowns();
    UPROPERTY(BlueprintReadOnly)
    TArray<UBaseTimeAbility*> ExecutingAbilities;
#pragma endregion Ability
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    /**
     * @brief flag for running character state
     */
    UPROPERTY()
    bool Running;
    
    /**
     * @brief Move this character depending of InputAction Value
     * @param InputValue Value of InputAction
     */
    UFUNCTION()
    void Move(const FInputActionValue& InputValue);


    /**
     * @brief change crouch state trigger when player pressed crouch button
     */
    void CrouchTriggered();

    void SetWalkSpeed(const float NewWalkSpeed) const;


#pragma  region Camera
    void RotateCamera(const FInputActionValue& InputValue);
    
    UPROPERTY()
    UTimelineComponent* CameraRotateTimeline;
    
    UFUNCTION()
    void CameraTimelineUpdate(float Value);
    
    UFUNCTION()
    void CameraTimelineFinishedCallback();
    
    void PlayCameraTimeline() const;
    void InitCameraTimeLine();

    float StartCameraRotateAngle;
    float FinisCameraRotateAngle;
    FRotator DefaultCameraRotation;
#pragma endregion Camera


#pragma region Ability
    UFUNCTION()
    void InitAbilities();
    UFUNCTION()
    void OnFinishAbility(UBaseTimeAbility* Ability);
    UFUNCTION()
    void OnCancelActiveAbility(UBaseTimeAbility* Ability);
    UFUNCTION()
    void ResetActiveAbility();
    UFUNCTION()
    void CommitAbility();
    UFUNCTION()
    void FinishCooldown(UCooldownEffect* Effect);
    UPROPERTY()
    TObjectPtr<UBaseTimeAbility> ActiveAbility;
    UPROPERTY(VisibleAnywhere)
    TMap<TSubclassOf<UBaseTimeAbility>,UCooldownEffect*> CooldownMap;
#pragma endregion Ability
};
