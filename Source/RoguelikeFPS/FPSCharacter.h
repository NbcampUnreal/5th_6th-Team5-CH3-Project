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

	// ===== 레벨업 델리게이트 =====
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUpSignature OnLevelUp;

	// ===== 사망 델리게이트 =====
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeathSignature OnPlayerDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventory* Inventory;//인벤토리 정보

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	// Status
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
	int32 shield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	bool bIsAlive;

	// 대시
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

	// 총 발사 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	bool bIsFiring;
	FTimerHandle FireCooltimeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	float FireCooltime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	float AutoFireTime;
	void PerformFire();

	// 장전 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	bool bIsReloading;
	FTimerHandle ReloadTimeHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	float ReloadTime;

	// Action Value

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartDash(const FInputActionValue& value);
	UFUNCTION()
	void StopDash();
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
	void StopReload();

	void LevelUp();
	void OnDeath(AController* KillerController);


	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
};