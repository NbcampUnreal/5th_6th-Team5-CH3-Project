#include "AIEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

const FName AAIEnemyController::BB_TargetActor = TEXT("TargetActor");
const FName AAIEnemyController::BB_InAttackRange = TEXT("InAttackRange");
const FName AAIEnemyController::BB_TargetDistance = TEXT("TargetDistance");
const FName AAIEnemyController::BB_HasLOS = TEXT("HasLineOfSight");

AAIEnemyController::AAIEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);
    }
}

void AAIEnemyController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!BlackboardComp) return;

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BB_TargetActor));
    if (!Target)
    {
        BlackboardComp->SetValueAsBool(BB_A, false);
        BlackboardComp->SetValueAsFloat(BB_TargetDistance, 0.f);
        BlackboardComp->SetValueAsBool(BB_HasLOS, false);
        return;
    }

    APawn* MyPawn = GetPawn();
    if (!MyPawn) return;

    const float Dist = FVector::Dist(MyPawn->GetActorLocation(), Target->GetActorLocation());
    BlackboardComp->SetValueAsFloat(BB_TargetDistance, Dist);

    const bool bInRange = (Dist <= AttackRange);
    BlackboardComp->SetValueAsBool(BB_InAttackRange, bInRange);

    const bool bLOS = LineOfSightTo(Target);
    BlackboardComp->SetValueAsBool(BB_HasLOS, bLOS);
}
