// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "Stage2BossAttackComponent.generated.h"

UENUM(BlueprintType)
enum class EHitShape : uint8
{
    Capsule UMETA(DisplayName = "Capsule (기존)"),
    Box     UMETA(DisplayName = "Box (사각형)"),
    Sector  UMETA(DisplayName = "Sector (부채꼴)")
};

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

    // ==== 새 옵션: 모양 선택 ====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    EHitShape HitShape = EHitShape::Capsule;

    // ==== Box 전용(반값: HalfExtent) ====
    // X=폭/2, Y=길이/2, Z=높이/2.  기본값은 기존 Range/Radius로부터 유도해도 됨.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Box")
    FVector BoxHalfExtent = FVector(60.f, 100.f, 60.f);

    // 박스 중심을 시작점에서 전방으로 얼마나 밀어낼지(길이/2 추천)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Box")
    float BoxForwardOffset = 100.f;

    // ==== Sector 전용 ====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Sector")
    float SectorRadius = 300.f;       // 부채꼴 반지름(보통 Range랑 같게)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Sector", meta = (ClampMin = "1", ClampMax = "180"))
    float SectorAngleDeg = 90.f;      // 전체 각도(좌우 합)

    // Z 높이 제한(수평 섹터만 쓰면 0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack|Sector")
    float SectorHalfHeight = 0.f;

    // 디버그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDebug = false;

    // 기존

    bool bAttackEnded = true;

    UPROPERTY(EditAnywhere, Category = "Debug|Melee")
    bool bDebugMeleeTrace = true;

    UPROPERTY(EditAnywhere, Category = "Debug|Melee", meta = (ClampMin = "0.0"))
    float DebugDrawTime = 0.25f;

    virtual void StartAttack(AActor* Target) override;
    virtual void DoHit() override;

private:
    UFUNCTION()
    void TraceAndApplyDamage();

    void DoCapsuleTraceAndDamage();   // 기존 로직 분리
    void DoBoxTraceAndDamage();       // 추가
    void DoSectorAndDamage();         // 추가
};
