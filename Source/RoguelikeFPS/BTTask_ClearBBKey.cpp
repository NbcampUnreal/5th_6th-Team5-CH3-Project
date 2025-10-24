
// BTTask_ClearBBKey.cpp
#include "BTTask_ClearBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ClearBBKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    if (auto* BB = OwnerComp.GetBlackboardComponent())
    {
        BB->ClearValue(KeyToClear.SelectedKeyName);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}