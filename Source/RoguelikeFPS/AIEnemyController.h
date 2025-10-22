#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

    // BT/BB 에셋은 BP에서 지정
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBlackboardComponent* BlackboardComp;

    // 공격 거리 기준 (원하는 값으로 튜닝)
    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackRange = 200.f;

    // 블랙보드 키 이름들
    static const FName BB_TargetActor;
    static const FName BB_InAttackRange;
    static const FName BB_TargetDistance;
    static const FName BB_HasLOS;
};
