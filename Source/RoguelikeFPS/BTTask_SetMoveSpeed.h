#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIEnemyCharacter.h"
#include "AIEnemyController.h"
#include "BTTask_SetMoveSpeed.generated.h"


UCLASS()
class ROGUELIKEFPS_API UBTTask_SetMoveSpeed : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector SpeedKey; // Float (선택: 없으면 Chase/Walk 함수 호출)

    UPROPERTY(EditAnywhere, Category = "Speed")
    bool bUseChaseSpeed = false;     // 체크 시 ChaseSpeed, 아니면 WalkSpeed

    UBTTask_SetMoveSpeed() { NodeName = TEXT("Set Move Speed"); }

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
    {
        if (AAIController* AICon = OwnerComp.GetAIOwner())
            if (AAIEnemyCharacter* Ch = Cast<AAIEnemyCharacter>(AICon->GetPawn()))
            {
                if (SpeedKey.SelectedKeyType) // BB에서 수치 직접 읽기
                {
                    const float S = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(SpeedKey.SelectedKeyName);
                    Ch->GetCharacterMovement()->MaxWalkSpeed = S;
                    Ch->CurrentMoveSpeed = S;
                }
                else
                {
                    if (bUseChaseSpeed) Ch->ApplyChaseSpeed();
                    else                Ch->ApplyWalkSpeed();
                }
                return EBTNodeResult::Succeeded;
            }
        return EBTNodeResult::Failed;
    }
};
