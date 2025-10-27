#pragma once
//JMS
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Structure/EnemyState.h"
#include "AI/Structure/EnemyConfig.h"
#include "AI/Structure/EnemyStateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIEnemyCharacter.generated.h"

UCLASS()
class ROGUELIKEFPS_API AAIEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAIEnemyCharacter();



protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

public:
    // 몹 BP에서 각자 다른 Asset 할당
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
    UEnemyConfig* Config = nullptr;   

    // FSM 컴포넌트 (상태 전이/상태별 로직 담당)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UEnemyStateMachineComponent* StateMachine;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Movement")
    float CurrentMoveSpeed = 0.f;

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    void ApplyWalkSpeed();     // 기본 순찰/이동 속도

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    void ApplyChaseSpeed();    // 추적 속도(있다면)




    // 디버그 기즈모 토글
    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    bool bDrawPerceptionDebug = true;

    // (선택) 라인 두께/세그먼트 조절
    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    int32 DebugSegments = 64;

    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    float DebugThickness = 1.0f;

    void DrawPerceptionGizmos();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MAXHP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float HP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float ATK;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DEF;


    // 체력 회복
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddHealth(float Amount);
    // 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void OnDeath();

    // 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
    // 또는 AActor의 TakeDamage()를 오버라이드
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
