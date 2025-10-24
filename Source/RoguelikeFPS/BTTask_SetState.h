#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyState.h"                          // ← 반드시 헤더에서 include
#include "BTTask_SetState.generated.h"

/**
 * Behavior Tree에서 FSM의 상태를 변경하는 Task
 * BT 노드 Details에 TargetState 드롭다운이 노출됨.
 */
UCLASS()
class ROGUELIKEFPS_API UBTTask_SetState : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SetState();

    // BT 노드에서 직접 선택할 목표 상태
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    EEnemyState TargetState = EEnemyState::Idle;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
