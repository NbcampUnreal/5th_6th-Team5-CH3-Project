// BTTask_ClearBBKey.h
#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearBBKey.generated.h"

UCLASS()
class ROGUELIKEFPS_API UBTTask_ClearBBKey : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector KeyToClear;

    UBTTask_ClearBBKey() { NodeName = TEXT("Clear BB Key"); }
protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};


