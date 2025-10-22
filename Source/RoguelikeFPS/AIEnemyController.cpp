#include "AIEnemyController.h"
#include "AIEnemyCharacter.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetMathLibrary.h"

const FName AAIEnemyController::BB_TargetActor = TEXT("TargetActor");
const FName AAIEnemyController::BB_HasLOS = TEXT("HasLineOfSight");
const FName AAIEnemyController::BB_TargetDistance = TEXT("TargetDistance");
const FName AAIEnemyController::BB_InAttackRange = TEXT("InAttackRange");
const FName AAIEnemyController::BB_LastSeenLocation = TEXT("LastSeenLocation");

AAIEnemyController::AAIEnemyController()
{
    PrimaryActorTick.bCanEverTick = true; // 거리/사거리 갱신을 Tick에서 유지

    // === Perception 구성 ===
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;

    // 누구를 '볼지' (팀 구분 필요 시 프로젝트에 맞게 조정)
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // '기억' 만료 시간
    SightConfig->SetMaxAge(ForgetDelaySeconds);

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    // 이벤트 바인딩
    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnTargetPerceptionUpdated);
    PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnPerceptionUpdated);
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);

        if (const auto* Enemy = Cast<AAIEnemyCharacter>(InPawn))
        {
            if (ensure(Enemy->Config))
            {
                // 공격 사거리
                AttackRange = Enemy->Config->AttackRange;

                // AIPerception 시야 파라미터 적용
                SightConfig->SightRadius = Enemy->Config->SightRadius;
                SightConfig->LoseSightRadius = Enemy->Config->LoseSightRadius;
                SightConfig->PeripheralVisionAngleDegrees = Enemy->Config->PeripheralVisionAngle;

                // 적용 갱신
                PerceptionComp->ConfigureSense(*SightConfig);     // 재적용
                PerceptionComp->RequestStimuliListenerUpdate();   // 갱신 요청
            }
        }
            
    }
}

void AAIEnemyController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!BlackboardComp) return;

    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BB_TargetActor));
    APawn* MyPawn = GetPawn();

    if (!Target || !MyPawn)
    {
        // 타깃 없으면 기본 초기화
        BlackboardComp->SetValueAsBool(BB_InAttackRange, false);
        BlackboardComp->SetValueAsFloat(BB_TargetDistance, 0.f);
        return;
    }

    // 2D 거리(지형 높낮이 무시)
    const float Dist = FVector::Dist2D(MyPawn->GetActorLocation(), Target->GetActorLocation());
    BlackboardComp->SetValueAsFloat(BB_TargetDistance, Dist);
    BlackboardComp->SetValueAsBool(BB_InAttackRange, Dist <= AttackRange);

    // HasLOS는 Perception 이벤트에서 갱신됨 (원한다면 여기서도 LineOfSightTo(Target)로 보조 갱신 가능)
}

void AAIEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!BlackboardComp) return;

    // 시야 센스만 처리
    if (Stimulus.Type != UAISense::GetSenseID(UAISense_Sight::StaticClass()))
        return;

    const bool bSensed = Stimulus.WasSuccessfullySensed();
    BlackboardComp->SetValueAsBool(BB_HasLOS, bSensed);

    if (bSensed)
    {
        // 본 즉시 타깃 지정 및 마지막 본 위치 갱신
        BlackboardComp->SetValueAsObject(BB_TargetActor, Actor);
        BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Actor->GetActorLocation());
    }
    else
    {
        // 이번 프레임에 '안 보임' 이벤트 발생.
        // 아직 활성 자극(Active Stimulus)이 남아 있는지 확인 → 없으면 완전 해제.
        FActorPerceptionBlueprintInfo Info;
        PerceptionComp->GetActorsPerception(Actor, Info);

        const bool bAnyActive =
            Info.LastSensedStimuli.ContainsByPredicate(
                [](const FAIStimulus& S) { return S.IsActive(); });

        if (!bAnyActive)
        {
            if (BlackboardComp->GetValueAsObject(BB_TargetActor) == Actor)
            {
                ClearTarget();
            }
        }
    }
}

void AAIEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    if (!BlackboardComp) return;

    // 선택: 현재 타깃을 지금 '보는 중'이면 LastSeenLocation 갱신
    if (AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BB_TargetActor)))
    {
        FActorPerceptionBlueprintInfo Info;
        PerceptionComp->GetActorsPerception(Target, Info);

        const bool bSensedNow =
            Info.LastSensedStimuli.ContainsByPredicate(
                [](const FAIStimulus& S) { return S.WasSuccessfullySensed(); });

        if (bSensedNow)
        {
            BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Target->GetActorLocation());
        }
    }
}

void AAIEnemyController::ClearTarget()
{
    BlackboardComp->ClearValue(BB_TargetActor);
    BlackboardComp->SetValueAsBool(BB_HasLOS, false);
    BlackboardComp->SetValueAsBool(BB_InAttackRange, false);
    BlackboardComp->SetValueAsFloat(BB_TargetDistance, 0.f);
    // LastSeenLocation은 유지하거나, 필요 시 (0,0,0)으로 초기화
}
