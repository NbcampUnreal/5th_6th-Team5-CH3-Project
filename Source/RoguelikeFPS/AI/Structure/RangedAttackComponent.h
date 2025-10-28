#pragma once
#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "RangedAttackComponent.generated.h"

class UAnimMontage;
class UProjectileMovementComponent;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API URangedAttackComponent : public UEnemyAttackBaseComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Attack|Melee")
    void ApplyRangeConfig(const UEnemyConfig* Config);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    float MuzzleSpeed = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    FName MuzzleSocket = TEXT("Muzzle");

    virtual void StartAttack(AActor* Target) override;
    virtual void DoHit() override;

private:
    void GetMuzzleTransform(FVector& OutLoc, FRotator& OutRot) const;
};
