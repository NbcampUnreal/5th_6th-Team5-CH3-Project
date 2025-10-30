#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

// 델리게이트: 레벨업 이벤트 발생 시 GameMode에게 알리기 위해 사용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, APlayerController*, PlayerController);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentXP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float XPToNextLevel = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 StatPointsAvailable = 0; // 선택 가능한 증강 횟수

public:
	// 델리게이트 선언: 레벨업 시 다른 시스템(GameMode)에 알림
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUp OnLevelUp;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddXP(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ApplyAugment(int32 AugmentIndex); // 증강을 적용하는 함수 (UI에서 호출)

private:
	void TryLevelUp();
};