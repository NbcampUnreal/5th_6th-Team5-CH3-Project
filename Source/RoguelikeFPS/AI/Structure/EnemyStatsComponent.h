// EnemyStatsComponent.h
#pragma once
#include "AI/Structure/EnemyConfig.h"
#include "EnemyStatsComponent.generated.h"


USTRUCT(BlueprintType)
struct FEnemyRuntimeStats
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Level = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxHP = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float HP = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float ATK = 10.f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float DEF = 0.f;
};

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class UEnemyStatsComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FEnemyRuntimeStats Stats;

    virtual void InitializeComponent() override;

    UFUNCTION(BlueprintCallable)
    void InitializeFromConfig(const class UEnemyConfig* Config)
    {
        if (!Config) return;
        // 레벨 결정(예: 1~2 증가)
        const int32 RolledLevel = Config->Level + FMath::RandRange(2, 2);
        UE_LOG(LogTemp, Log, TEXT("RolledLevel %d"), RolledLevel);
        Stats.Level = RolledLevel;
        Stats.MaxHP = Config->MaxHP + (RolledLevel) * (Config->HPperLevel);
        Stats.HP = Stats.MaxHP;
        Stats.ATK = Config->ATK + (RolledLevel) * (Config->ATKperLevel);
        UE_LOG(LogTemp, Log, TEXT("ATK : %f"), GetATK());
        Stats.DEF = Config->DEF + (RolledLevel) * (Config->DEFperLevel);
    }

    // 편의 Getters
    FORCEINLINE int32 GetLevel() const { return Stats.Level; }
    FORCEINLINE float GetATK() const { return Stats.ATK; }
    FORCEINLINE float GetDEF() const { return Stats.DEF; }
    FORCEINLINE float GetHP()  const { return Stats.HP; }
    FORCEINLINE float GetMaxHP()const { return Stats.MaxHP; }
    // 필요시 HP 증감, 사망 판정 메서드 추가
};
