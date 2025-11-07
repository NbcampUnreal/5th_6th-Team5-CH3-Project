#include "BTTask_SetState.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AI/Structure/Component/EnemyStateMachineComponent.h"          // FSM 컴포넌트
#include "AI/Character/AIEnemyCharacter.h"                    // 캐릭터 클래스(컴포넌트 찾기용)

UBTTask_SetState::UBTTask_SetState()
{
    NodeName = TEXT("Set Enemy State");
    // bCreateNodeInstance = true;  // ← 쓰지 마세요. (값 리셋 원인)
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[BTTask_SetEnemyState] No AIController"));
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[BTTask_SetEnemyState] No Pawn"));
        return EBTNodeResult::Failed;
    }

    // 캐릭터에서 FSM 컴포넌트를 찾음 (캐릭터 타입이 다르면 FindComponentByClass만 써도 무방)
    UEnemyStateMachineComponent* FSM = Pawn->FindComponentByClass<UEnemyStateMachineComponent>();
    if (!FSM)
    {
        //UE_LOG(LogTemp, Warning, TEXT("[BTTask_SetEnemyState] FSM component not found on %s"), *Pawn->GetName());
        return EBTNodeResult::Failed;
    }

    //UE_LOG(LogTemp, Warning, TEXT("[BTTask_SetEnemyState] Change to: %s"),*UEnum::GetValueAsString(TargetState));

    FSM->ChangeState(TargetState);
    return EBTNodeResult::Succeeded;
}
