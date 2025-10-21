#include "AIEnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyStateMachineComponent.h"

AAIEnemyCharacter::AAIEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->SightRadius = 1000.0f;
    PawnSensingComp->SetPeripheralVisionAngle(45.0f);
    PawnSensingComp->SetSensingUpdatesEnabled(true);

    PawnSensingComp->bOnlySensePlayers = false; //임시
}

void AAIEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    //Controller를 얻어왔는지 확인
    if (GetController())
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller OK"));
    }

    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller is NULL!"));
    }


    if (PawnSensingComp)
        PawnSensingComp->OnSeePawn.AddDynamic(this, &AAIEnemyCharacter::OnSeePawn);
}

void AAIEnemyCharacter::OnSeePawn(APawn* Pawn)
{
    if (!Pawn) return;

    TargetActor = Pawn;

    if (AAIEnemyController* AIController = Cast<AAIEnemyController>(GetController()))
    {
        // BT의 Blackboard에 타겟 설정
        if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
        {
            BB->SetValueAsObject(TEXT("TargetActor"), TargetActor);
        }

        // FSM 상태 변경
        if (UEnemyStateMachineComponent* FSM = FindComponentByClass<UEnemyStateMachineComponent>())
        {
            FSM->ChangeState(EEnemyState::Chase);
        }
    }
}
