#pragma once
//JMS
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Structure/EnemyState.h"
#include "EnemyStateMachineComponent.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UMeleeAttackComponent;
class URangedAttackComponent;
class UStage2BossAttackComponent;
class UEnemyAnimInstance;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROGUELIKEFPS_API UEnemyStateMachineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnemyStateMachineComponent();


    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //BB에서 가져온 TargetActor
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    FName BBKey_TargetActor = TEXT("TargetActor");
    //근접 사거리
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    float MeleeThreshold = 350.f;

    // 현재 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
    EEnemyState CurrentState;

    // 상태 전환 함수
    UFUNCTION(BlueprintCallable, Category = "FSM")
    void ChangeState(EEnemyState NewState);

    //BB에서 TargetActor 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "AI")
    AActor* GetTargetFromBlackboard() const;

    FTimerHandle PostAttackTimer;
    UPROPERTY(EditAnywhere, Category = "Combat")
    float PostAttackDelay = 0.3f;   // 공격 후 짧은 대기(블렌딩/리커버리용)

    FTimerHandle AttackRecheckTimer;
    void RecheckAttackReady();  // 쿨타임 대기 중 재평가

private:
    TWeakObjectPtr<UMeleeAttackComponent>  MeleeComp;
    TWeakObjectPtr<URangedAttackComponent> RangedComp;
    TWeakObjectPtr<UStage2BossAttackComponent> Boss2Comp;

    UFUNCTION()
    void HandleAttackFinished();

    void SetCombatLocked(bool bLocked);

    float Dist2DToTarget(AActor* Target) const;

    UPROPERTY(Transient)
    UEnemyAnimInstance* CachedAnim = nullptr;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    // 상태 진입 / 종료 / 유지 처리
    virtual void OnEnterState(EEnemyState State);
    virtual void OnExitState(EEnemyState State);
    virtual void TickState(EEnemyState State, float DeltaTime);
    virtual void OnEnter_Attack();

    UFUNCTION()
    void OnMeleeFinished();
    UFUNCTION()
    void OnRangedFinished();
    UFUNCTION()
    void OnBoss2Finished();

    void FinalizeAfterAttack();
    void OnPostAttackDelayElapsed();
};
