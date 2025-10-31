// StatsComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

// 델리게이트: 레벨업 이벤트 발생 시 GameMode에게 알리기 위해 사용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, APlayerController*, PlayerController);
// 체력 변경 알림 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, NewHealth, int32, MaxHealth);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();

protected:
	virtual void BeginPlay() override;

protected:
	// Core Stats (Health & Movement)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Core")
	int32 CurrentHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Core")
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Core")
	float BaseMovementSpeed = 600.0f; // 캐릭터 이동속도 관리용

	// Combat Stats (Attack & Defense)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
	float DefenseValue = 5.0f; // 데미지 감소량

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
	float CritChance = 0.05f; // 치명타 확률 (5%)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Combat")
	float CritDamageMultiplier = 1.5f; // 치명타 대미지 배율 (150%)


	// Experience Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Experience")
	float CurrentXP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Experience")
	float XPToNextLevel = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Experience")
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats|Experience")
	int32 StatPointsAvailable = 0; // 선택 가능한 증강 횟수 <- 한번에 2레벨 이상 업 했을 경우 1번만 뜨는 것을 방지하기 위해

public:
	// 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUp OnLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	// BlueprintPure (Getter) 함수
	UFUNCTION(BlueprintPure, Category = "Stats|Core")
	int32 GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Stats|Core")
	float GetMovementSpeed() const { return BaseMovementSpeed; }

	UFUNCTION(BlueprintPure, Category = "Stats|Core")
	float GetDefenseValue() const { return DefenseValue; }

	UFUNCTION(BlueprintPure, Category = "Stats|Core")
	int32 GetLevel() const { return CurrentLevel; }

	// 핵심 로직 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float CalculateFinalDamage(float BaseDamage, bool& bWasCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float ApplyDamage(float DamageToApply);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Heal(int32 HealAmount);

	// Experience Functions
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddXP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ApplyAugment(int32 AugmentIndex); // 증강을 적용하는 함수 (UI에서 호출)

private:
	void TryLevelUp();
	void ClampHealth();
};