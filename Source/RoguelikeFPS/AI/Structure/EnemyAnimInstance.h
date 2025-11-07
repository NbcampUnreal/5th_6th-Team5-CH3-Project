#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI/Structure/EnemyState.h"
#include "EnemyAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPattern : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Pattern1 UMETA(DisplayName = "Pattern1"),
    Pattern2 UMETA(DisplayName = "Pattern2"),
    Pattern3 UMETA(DisplayName = "Pattern3"),

};

UCLASS()
class ROGUELIKEFPS_API UEnemyAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
public:
    // FSM이 건드릴 변수들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    EEnemyState AnimState = EEnemyState::Idle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bHasTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bInAttack = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bIsStun = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    EPattern PatternState = EPattern::Pattern1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    float MoveSpeed = 0.f;

    // FSM에서 바로 쓰기 좋은 헬퍼
    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetAnimState(EEnemyState NewState) { AnimState = NewState; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetHasTarget(bool bNew) { bHasTarget = bNew; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetInAttack(bool bNew) { bInAttack = bNew; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetIsDead(bool bNew) { bIsDead = bNew; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetIsStun(bool bNew) { bIsStun = bNew; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetPattern(EPattern NewState) { PatternState = NewState; }

    UFUNCTION(BlueprintCallable, Category = "Anim")
    void SetMoveSpeed(float InSpeed) { MoveSpeed = InSpeed; }

};
