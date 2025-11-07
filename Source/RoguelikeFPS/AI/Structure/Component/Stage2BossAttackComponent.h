// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "Stage2BossAttackComponent.generated.h"


class UEnemyConfig;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UStage2BossAttackComponent : public UEnemyAttackBaseComponent
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable, Category = "Attack|Melee")
    void ApplyMeleeConfig(const UEnemyConfig* Config, float ConfigDamage);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float Range = 220.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float Radius = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float AttackRange = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float AddtionalRange = 0.f;

    float GetAttackRange() { return AttackRange; }

    UPROPERTY(EditAnywhere, Category = "Attack|Trace")
    TEnumAsByte<ECollisionChannel> MeleeTraceChannel = ECC_Visibility; // 권장: Visibility

    UPROPERTY(EditAnywhere, Category = "Attack|Trace")
    bool bTraceComplex = false;

    UPROPERTY(EditAnywhere, Category = "Attack|Trace")
    bool bUseLineOfSightCheck = false; // 원하면 추가 LOS 확인

    // 한 번의 공격 동안만 유지 → Notify_End 등에서 Clear
    TSet<TWeakObjectPtr<AActor>> HitActorsThisSwing;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void BeginAttackWindow() { HitActorsThisSwing.Reset(); bAttackEnded = false; }

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void EndAttackWindow() { HitActorsThisSwing.Reset(); bAttackEnded = true; }

    // 기존

    bool bAttackEnded = true;

    UPROPERTY(EditAnywhere, Category = "Debug|Melee")
    bool bDebugMeleeTrace = true;

    UPROPERTY(EditAnywhere, Category = "Debug|Melee", meta = (ClampMin = "0.0"))
    float DebugDrawTime = 0.25f;

    int patternNum = 0;

    virtual void StartAttack(AActor* Target) override;
    virtual void DoHit() override;

private:
    UFUNCTION()
    void TraceAndApplyDamage();

    void DoCapsuleTraceAndDamage();   // 기존 로직 분리
};
