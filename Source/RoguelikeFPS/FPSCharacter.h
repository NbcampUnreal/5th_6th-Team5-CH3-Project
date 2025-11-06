#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpSignature, APlayerController*, PlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeathSignature, AController*, KillerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDStatChangedSignature, FName, StatName); // HUD 업데이트 신호

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AddXP(float Amount);

	UFUNCTION(BlueprintCallable)
	void ApplyAugment(FName AugmentName);

	//HUD 업데이트를 외부에서 호출할 경우 사용
	void UpdateHUDStats(FName StatName);

	// ===== 델리게이트 =====
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUpSignature OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeathSignature OnPlayerDeath;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHUDStatChangedSignature OnHUDStatChanged;

	// --- HUD Getter 함수들 (StatsHUD에서 접근 가능하도록 PUBLIC) ---
	UFUNCTION(BlueprintCallable, Category = "CharacterStatus")
	void GainGold(int32 Amount);
	UFUNCTION(BlueprintPure, Category = "CharacterStatus")
	int32 GetHealth() const { return Health; }
	UFUNCTION(BlueprintPure, Category = "CharacterStatus")
	int32 GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintPure, Category = "CharacterStatus")
	int32 GetGoldAmount() const { return GoldAmount; }
	UFUNCTION(BlueprintPure, Category = "WeaponInfo")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }
	UFUNCTION(BlueprintPure, Category = "WeaponInfo")
	int32 GetMaxAmmo() const { return MaxAmmo; }
	UFUNCTION(BlueprintPure, Category = "CharacterStatus")
	float GetCurrentExperience() const { return CurrentExperience; }
	UFUNCTION(BlueprintPure, Category = "CharacterStatus")
	int32 GetMaxExperience() const { return MaxExperience; }
	UFUNCTION(BlueprintPure, Category = "WeaponInfo")
	FName GetCurrentWeaponName() const { return CurrentWeaponName; }
	UFUNCTION(BlueprintPure, Category = "Skills")
	float GetSkill1CooldownRemaining() const { return Skill1CooldownRemaining; }
	UFUNCTION(BlueprintPure, Category = "Skills")
	float GetSkill2CooldownRemaining() const { return Skill2CooldownRemaining; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	// Status (기존 변수들)
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

	// --- HUD 관련 변수 ---
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

	// --- Dash 관련 코드 ---
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

	// --- 총/장전 관련 ---
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

	// Action Value 함수 선언들
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
	void OnDeath(AController* KillerController);


	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaTime) override; // Tick 활성화
};