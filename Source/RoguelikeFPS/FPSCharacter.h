#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStageClear);

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFPSCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ====== 스탯 ======
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 PlayerLevel = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentXP = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float MaxXP = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MaxHealth = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Attack = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Defence = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 AttackSpeed = 5;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 MovingSpeed = 600;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
    bool bIsAlive = true;

    // ====== 카메라 ======
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> CameraComp;

    // ====== 무기 ======
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
    USkeletalMeshComponent* PistolMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
    USkeletalMeshComponent* RifleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
    USkeletalMeshComponent* ShotgunMesh;

    // ====== 대시 ======
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
    float DashMultiplier = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
    float DashTime = 1.0f;

    float DashSpeed;
    FTimerHandle DashTimerHandle;

    // ====== 델리게이트 ======
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerLevelUp OnPlayerLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerDeath OnPlayerDeath;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerStageClear OnPlayerStageClear;

    // ====== 함수 ======
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddXP(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LevelUp();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ApplyAugment(FName AugmentID);

    UFUNCTION(BlueprintCallable, Category = "Character")
    void HandleDeath();

    UFUNCTION(BlueprintCallable, Category = "Character")
    void HandleStageClear();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void InitializeWeapon(FName SelectedWeaponName);

    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void Look(const FInputActionValue& Value);

    UFUNCTION()
    void StartJump(const FInputActionValue& Value);

    UFUNCTION()
    void StopJump(const FInputActionValue& Value);

    UFUNCTION()
    void StartDash(const FInputActionValue& Value);

    UFUNCTION()
    void StopDash();

    virtual float TakeDamage(
        float DamageAmount,
        FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser
    ) override;

    void FireWeapon();
};
