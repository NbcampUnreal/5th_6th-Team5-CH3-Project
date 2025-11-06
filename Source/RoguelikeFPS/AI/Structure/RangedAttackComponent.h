#pragma once
#include "CoreMinimal.h"
#include "EnemyAttackBaseComponent.h"
#include "RangedAttackComponent.generated.h"

class UAnimMontage;
class UProjectileMovementComponent;
class AProjectile;

UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API URangedAttackComponent : public UEnemyAttackBaseComponent
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Attack|Melee")
    void ApplyRangeConfig(const UEnemyConfig* Config, float ConfigDamage);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    TSubclassOf<AProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    float MuzzleSpeed = 2000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    FName MuzzleSocket = TEXT("Muzzle");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    float CorrectX = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    float CorrectY = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ranged")
    float CorrectZ = 0.f;

    virtual void StartAttack(AActor* Target) override;
    virtual void DoHit() override;

private:
    void GetMuzzleTransform(FVector& OutLoc, FRotator& OutRot) const;
};
