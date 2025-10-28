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
    void ApplyMeleeConfig(const UEnemyConfig* Config);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float Range = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee")
    float Radius = 60.f;

    virtual void StartAttack(AActor* Target) override;
    virtual void DoHit() override;

private:
    UFUNCTION()
    void TraceAndApplyDamage();
};
