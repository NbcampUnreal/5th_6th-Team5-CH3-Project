#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Structure/EnemyState.h"
#include "AI/Structure/EnemyConfig.h"
#include "AI/Structure/Component/EnemyStateMachineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../DropTable.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    UDataTable* DropTable;

    //삭제 예정
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> DropItemClass;

    UFUNCTION(BlueprintCallable, Category = "AI|Config")
    void ApplyConfigToComponents(float Damage);

    // FSM 컴포넌트 (상태 전이/상태별 로직 담당)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UEnemyStateMachineComponent* StateMachine;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Movement")
    float CurrentMoveSpeed = 0.f;

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    void ApplyWalkSpeed();     // 기본 순찰/이동 속도

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    void ApplyChaseSpeed();    // 추적 속도(있다면)

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    float GetWalkSpeed();     // 기본 순찰/이동 속도

    UFUNCTION(BlueprintCallable, Category = "AI|Movement")
    float GetChaseSpeed();    // 추적 속도(있다면)

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teleport")
    AActor* TeleportSpawnLocation;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teleport")
    TSubclassOf<AActor> TeleportSpawnActor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teleport")
    FName NextLevelName = TEXT("LevelName");


    // 디버그 기즈모 토글
    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    bool bDrawPerceptionDebug = false;

    // (선택) 라인 두께/세그먼트 조절
    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    int32 DebugSegments = 64;

    UPROPERTY(EditAnywhere, Category = "AI|Debug")
    float DebugThickness = 1.0f;

    void DrawPerceptionGizmos();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    int32 Level;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float MAXHP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float HP;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float ATK;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
    float DEF;




    UPROPERTY(EditAnywhere, Category = "Combat")
    float HeadshotMultiplier = 2.0f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSet<FName> HeadBones = { TEXT("head"), TEXT("Head"), TEXT("Head_top") /* 등 프로젝트 본명에 맞춰 */ };

    UFUNCTION(BlueprintCallable)
    bool IsHeadBone(FName Bone) const { return HeadBones.Contains(Bone); }


    // 체력 회복
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddHealth(float Amount);
    // 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void OnDeath();

    bool bIsStunned = false;

    FTimerHandle StunTimerHandle;

    UFUNCTION(BlueprintCallable)
    void ApplyStun(float Duration);

    UFUNCTION(BlueprintCallable)
    void DropItem();



    // 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
    // 또는 AActor의 TakeDamage()를 오버라이드
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
