// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Node/BTTask_ClearLastSeen.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearLastSeen::UBTTask_ClearLastSeen()
{
    NodeName = TEXT("Clear LastSeen");
    LastSeenVectorKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ClearLastSeen, LastSeenVectorKey));
    HasLastSeenKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ClearLastSeen, HasLastSeenKey));
}

EBTNodeResult::Type UBTTask_ClearLastSeen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
    {
        if (!ensure(!HasLastSeenKey.SelectedKeyName.IsNone()))
            return EBTNodeResult::Failed;

        // Vector도 마찬가지
        if (!ensure(!LastSeenVectorKey.SelectedKeyName.IsNone()))
            return EBTNodeResult::Failed;

        BB->SetValueAsVector(LastSeenVectorKey.SelectedKeyName, FVector::ZeroVector);
        BB->SetValueAsBool(HasLastSeenKey.SelectedKeyName, false);

        // 즉시 확인 로그
        UE_LOG(LogTemp, Warning, TEXT("[ClearLastSeen] Set HasLastSeen=false  Key=%s"),
            *HasLastSeenKey.SelectedKeyName.ToString());
        UE_LOG(LogTemp, Warning, TEXT("[ClearLastSeen] After HasLastSeen=%s"),
            BB->GetValueAsBool(HasLastSeenKey.SelectedKeyName) ? TEXT("true") : TEXT("false"));

        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}