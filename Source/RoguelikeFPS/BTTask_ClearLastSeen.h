// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearLastSeen.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKEFPS_API UBTTask_ClearLastSeen : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector LastSeenVectorKey; // Vector
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector HasLastSeenKey;    // Bool
    UBTTask_ClearLastSeen();
protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
