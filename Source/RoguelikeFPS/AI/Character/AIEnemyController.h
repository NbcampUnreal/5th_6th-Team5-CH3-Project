#pragma once
//JMS
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyController.generated.h"

UCLASS()
class ROGUELIKEFPS_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()

public:
    AAIEnemyController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "AI|Filter")
    FName DisallowedTargetTag = TEXT("enemy");


    /** BT/BB 자산은 BP_EnemyController에서 이 필드에 지정 */
    UPROPERTY(EditDefaultsOnly, Category = "AI|BT")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|BT")
    UBlackboardComponent* BlackboardComp;

    /** === Perception (Sight) === */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    UAIPerceptionComponent* PerceptionComp;

    UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

    /** Perception 설정값 */
    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float SightRadius = 2000.f;

    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float LoseSightRadius = 2500.f;      // Sight보다 크게

    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float PeripheralVisionAngle = 90.f;  // 시야각

    /** '기억' 만료 시간: 이 시간 동안 못 보면 자극이 자동 만료됨 */
    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float ForgetDelaySeconds = 2.0f;

    /** 전투/추적 파라미터 */
    UPROPERTY(EditAnywhere, Category = "AI|Combat")
    float AttackRange = 200.f;           // InAttackRange 판정용

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
    UAISenseConfig_Hearing* HearingConfig = nullptr;

    // 튜닝값 (Config에서 받아도 OK)
    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float HearingRange = 2000.f;

    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float LoSHearingRange = 2500.f;

    UPROPERTY(EditAnywhere, Category = "AI|Perception")
    float HearingForgetSeconds = 3.f;




    UFUNCTION()
    void SendDeadToBT();

    /** BB 키 이름들 (한 곳에서 관리) */
    static const FName BB_TargetActor;
    static const FName BB_HasLOS;
    static const FName BB_TargetDistance;
    static const FName BB_InAttackRange;
    static const FName BB_LastSeenLocation;
    static const FName BB_HasLastSeen;
    static const FName BB_LastHeardLocation;
    static const FName BB_InSoundRange;
    //static const FName BB_WasDamagedRecently;

protected:
    /** Perception 이벤트 */
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    // 피격 시 가해자 추격
    UFUNCTION()
    void OnTakeAnyDamage_Handled(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void ClearDamagedFlag();

    /** 타깃 및 관련 키 초기화 */
    FTimerHandle Timer_DamagedFlagReset;
    void ClearTarget();
};
