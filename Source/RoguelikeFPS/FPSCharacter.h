#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInventory;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, APlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeathSignature, AController*, KillerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDStatChangedSignature, FName, StatName);

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AFPSCharacter();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    UFUNCTION(BlueprintCallable)
    void AddXP(float Amount);
    UFUNCTION(BlueprintCallable, Category = "Augment")
    void ApplyAugment(FName AugmentName);
    UFUNCTION(BlueprintCallable)
    void UpdateHUDStats(FName StatName);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void GainGold(int32 Amount);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLevelUpSignature OnLevelUp;
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerDeathSignature OnPlayerDeath;
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHUDStatChangedSignature OnHUDStatChanged;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    UInventory* Inventory;//인벤토리 정보

    int32 GetLevel() const { return Level; }
    int32 GetHealth() const { return Health; }
    int32 GetMaxHealth() const { return MaxHealth; }
    int32 GetAttack() const { return Attack; }
    int32 GetDefence() const { return Defence; }
    int32 GetShield() const { return Shield; }
    int32 GetAttackSpeed() const { return AttackSpeed; }
    int32 GetMovingSpeed() const { return MovingSpeed; }
    int32 GetDashSpeed() const { return DashSpeed; }
    int32 GetStamina() const { return Stamina; }
    int32 GetExperience() const { return Experience; }
    int32 GetMaxExperience() const { return MaxExperience; }
    FName GetCurrentWeaponName() const { return CurrentWeaponName; }
    int32 GetCurrentAmmo() const { return CurrentAmmo; }
    int32 GetMaxAmmo() const { return MaxAmmo; }
    float GetSkill1CooldownRemaining() const { return Skill1CooldownRemaining; }
    float GetSkill2CooldownRemaining() const { return Skill2CooldownRemaining; }
    int32 GetGoldAmount() const { return GoldAmount; }
    bool GetIsDash() const { return bIsDashing; }

    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetLevel(int32 level);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetHealth(int32 health);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetMaxHealth(int32 maxHealth);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetAttack(int32 attack);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetDefence(int32 defence);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetShield(int32 shield);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetAttackSpeed(int32 attackSpeed);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetMovingSpeed(int32 movingSpeed);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetStamina(int32 stamina);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetExperience(int32 experience);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetMaxExperience(int32 maxExperience);
    UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
    void SetIsDashing(bool isdash);

<<<<<<< Updated upstream
=======
    // 무적 상태변수
    bool Undead = false;
    // 무적 지속 시간
    FTimerHandle UndeadTimeHandle;
    // 무적 함수
    void OnUndead();
    void OffUndead();
    void OnUndeadTime(float time);

>>>>>>> Stashed changes
protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArmComp;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Level;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Health;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 MaxHealth;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Attack;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Defence;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 AttackSpeed;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 MovingSpeed;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Stamina;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Experience;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 MaxExperience;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    bool bIsAlive;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatus")
    int32 GoldAmount = 100;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatus")
    int32 CurrentAmmo = 30;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatus")
    int32 MaxAmmo = 300;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterStatus")
    float CurrentExperience = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
    float Skill1CooldownRemaining = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
    float Skill2CooldownRemaining = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponInfo")
    FName CurrentWeaponName = TEXT("Pistol");
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
    int32 Shield = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
    float DashMultifly;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
    float DashSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
    float DashTime;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
    bool bIsDashing;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
    bool bIsDashing2;
    FTimerHandle DashTimerHandle;

    UFUNCTION()
    void StartDash(const FInputActionValue& value);
    UFUNCTION()
    void StopDash();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
    bool bIsFiring;
    FTimerHandle FireCooltimeHandle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
    float FireCooltime;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
    float AutoFireTime;
    void PerformFire();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
    bool bIsReloading;
    FTimerHandle ReloadTimeHandle;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
    float ReloadTime;
    void StopReload();

    UFUNCTION()
    void Move(const FInputActionValue& value);
    UFUNCTION()
    void Look(const FInputActionValue& value);
    UFUNCTION()
    void StartJump(const FInputActionValue& value);
    UFUNCTION()
    void StopJump(const FInputActionValue& value);
    UFUNCTION()
    void StartCrouch(const FInputActionValue& value);
    UFUNCTION()
    void StopCrouch(const FInputActionValue& value);
    UFUNCTION()
    void Fire(const FInputActionValue& value);
    UFUNCTION()
    void StopFire();
    UFUNCTION()
    void StartFire_Auto(const FInputActionValue& value);
    UFUNCTION()
    void StopFire_Auto(const FInputActionValue& value);
    UFUNCTION()
    void Reload(const FInputActionValue& value);
    void LevelUp();
    UFUNCTION(BlueprintCallable)
    void OnDeath(AController* KillerController);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Augment")
    TArray<FName> AppliedAugments;

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    virtual void Tick(float DeltaTime) override;
};