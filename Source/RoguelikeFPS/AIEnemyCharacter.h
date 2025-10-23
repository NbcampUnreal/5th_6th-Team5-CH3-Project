#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyState.h"
#include "EnemyConfig.h"
#include "EnemyStateMachineComponent.h"
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
};
