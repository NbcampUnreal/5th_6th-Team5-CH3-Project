#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatsComponent.h"
#include "FPSCharacter.generated.h"

// AController 포인터를 포함하는 사망 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, AController*, KillerController);

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class ROGUELIKEFPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFPSCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatsComponent> StatsComp;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerDeath OnPlayerDeath;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Status")
	void Die(AController* KillerController); // 사망 함수

	//피격 함수
	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

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
	// 캐릭터 생존여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	bool bIsAlive;


	// 대시 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashMultifly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashTime;


	// 대시 타이머 핸들러
	FTimerHandle DashTimerHandle;


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

	// 레벨업
	//void LevelUp();

	// 사망
	//void OnDeath();
};

