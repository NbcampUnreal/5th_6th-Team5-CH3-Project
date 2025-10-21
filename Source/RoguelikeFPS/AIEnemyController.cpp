#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);
    }
}

void AAIEnemyController::SetTargetActor(AActor* Target)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject("TargetActor", Target);
    }
}
