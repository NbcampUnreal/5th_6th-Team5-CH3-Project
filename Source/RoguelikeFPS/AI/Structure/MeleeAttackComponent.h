#pragma once
#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "MeleeAttackComponent.generated.h"

class UAnimMontage;
class UEnemyConfig;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UMeleeAttackComponent : public UEnemyAttackBaseComponent
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
};
