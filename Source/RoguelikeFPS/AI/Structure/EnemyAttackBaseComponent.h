#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackBaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackFinished);

class UEnemyConfig;

UCLASS(Abstract, Blueprintable, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UEnemyAttackBaseComponent : public UActorComponent
{
    GENERATED_BODY()
public:


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
    float Damage = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
    float Cooldown = 1.0f;

    UPROPERTY(BlueprintAssignable, Category = "Attack")
    FOnAttackFinished OnAttackFinished;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void ApplyConfig(const UEnemyConfig* Config, float ConfigDamage);

    float GetCurrentDamage() const;



    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual bool CanAttack() const;

    // FSM OnEnter(Attack)에서 호출
    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void StartAttack(AActor* Target);

    // AnimNotify에서 호출(히트 시점)
    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void DoHit();

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void CancelAttack();

    UFUNCTION(BlueprintCallable, Category = "Attack")
    virtual void FinishAttack(); // 쿨다운 시작 + 델리게이트 브로드캐스트

protected:

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentTarget;

    UPROPERTY()
    bool bCanceled = false;

    UPROPERTY()
    float NextReadyTime = 0.f;

    UPROPERTY()
    bool bAttackInProgress = false;

    void MarkCooldown();
    bool HasServerAuthority() const;
};
