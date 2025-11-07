#include "AI/Character/AIEnemyController.h"
#include "AI/Character/AIEnemyCharacter.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/KismetMathLibrary.h"

const FName AAIEnemyController::BB_TargetActor = TEXT("TargetActor");
const FName AAIEnemyController::BB_HasLOS = TEXT("HasLineOfSight");
const FName AAIEnemyController::BB_TargetDistance = TEXT("TargetDistance");
const FName AAIEnemyController::BB_InAttackRange = TEXT("InAttackRange");
const FName AAIEnemyController::BB_LastSeenLocation = TEXT("LastSeenLocation");
const FName AAIEnemyController::BB_HasLastSeen = TEXT("HasLastSeen");
const FName AAIEnemyController::BB_LastHeardLocation = TEXT("LastHeardLocation");
const FName AAIEnemyController::BB_InSoundRange = TEXT("InSoundRange");


AAIEnemyController::AAIEnemyController()
{
    PrimaryActorTick.bCanEverTick = true; // 거리/사거리 갱신을 Tick에서 유지

    // === Perception 구성 ===
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;

    // 누구를 '볼지' (팀 구분 필요 시 프로젝트에 맞게 조정)
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // '기억' 만료 시간
    SightConfig->SetMaxAge(ForgetDelaySeconds);


    HearingConfig->HearingRange = HearingRange;
    HearingConfig->LoSHearingRange = LoSHearingRange;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->SetMaxAge(HearingForgetSeconds);


    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->ConfigureSense(*HearingConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    // 이벤트 바인딩
    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnTargetPerceptionUpdated);
    PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnPerceptionUpdated);
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (!BehaviorTreeAsset) return;

    UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
    RunBehaviorTree(BehaviorTreeAsset);

    if (InPawn && BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
        InPawn->OnTakeAnyDamage.AddDynamic(this, &AAIEnemyController::OnTakeAnyDamage_Handled);
    }

    const auto* Enemy = Cast<AAIEnemyCharacter>(InPawn);
    if (!ensure(Enemy && Enemy->Config && PerceptionComp)) return;

    if (!SightConfig)
    {
        // 없으면 런타임에 새로 만든다
        SightConfig = NewObject<UAISenseConfig_Sight>(this, UAISenseConfig_Sight::StaticClass());
        // 기본 감지 대상 설정(프로젝트 규칙에 맞게)
        SightConfig->DetectionByAffiliation.bDetectEnemies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

        // Perception에 등록 + 우선 감각으로 지정
        PerceptionComp->ConfigureSense(*SightConfig);
        PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
    }

    // ★ 이제 안전하게 몹별 Config 값 적용
    SightConfig->SightRadius = Enemy->Config->SightRadius;
    SightConfig->LoseSightRadius = Enemy->Config->LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = Enemy->Config->PeripheralVisionAngle;
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 0.f; // 시야 스티킹 방지 권장
    SightConfig->SetMaxAge(ForgetDelaySeconds);

    // 변경 반영
    PerceptionComp->RequestStimuliListenerUpdate();

    // 나머지 값들
    AttackRange = Enemy->Config->AttackRange;
    BlackboardComp->SetValueAsFloat(TEXT("PatrolRadius"), Enemy->Config->PatrolRadius);
    BlackboardComp->SetValueAsFloat(TEXT("PatrolWaitMin"), Enemy->Config->PatrolWaitMin);
    BlackboardComp->SetValueAsFloat(TEXT("PatrolWaitMax"), Enemy->Config->PatrolWaitMax);
    BlackboardComp->SetValueAsFloat(TEXT("AcceptanceRadius"), Enemy->Config->AcceptanceRadius);
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

    // 여기서 캐릭터만 감지하게 처리
    if (Actor->ActorHasTag(DisallowedTargetTag))
    {
        // 혹시 이미 타깃으로 잡혀 있던 경우도 즉시 해제
        if (BlackboardComp->GetValueAsObject(BB_TargetActor) == Actor)
        {
            BlackboardComp->ClearValue(BB_TargetActor);
            BlackboardComp->SetValueAsBool(BB_HasLOS, false);
        }
        return;
    }

    // 시야 센스만 처리
    if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
    {
        const bool bSensed = Stimulus.WasSuccessfullySensed();
        BlackboardComp->SetValueAsBool(BB_HasLOS, bSensed);

        if (bSensed)
        {
            // 본 즉시 타깃 지정 및 마지막 본 위치 갱신
            BlackboardComp->SetValueAsObject(BB_TargetActor, Actor);
            //BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Actor->GetActorLocation());
            BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Stimulus.StimulusLocation);
            BlackboardComp->SetValueAsBool(BB_HasLastSeen, true);
        }
        else
        {
            // 이번 프레임에 '안 보임' 이벤트 발생.
            // 아직 활성 자극(Active Stimulus)이 남아 있는지 확인 → 없으면 완전 해제.

            BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Stimulus.StimulusLocation);

            FActorPerceptionBlueprintInfo Info;
            PerceptionComp->GetActorsPerception(Actor, Info);

            const bool bAnyActive =
                Info.LastSensedStimuli.ContainsByPredicate(
                    [](const FAIStimulus& S) { return S.IsActive(); });

            if (!bAnyActive)
            {
                if (BlackboardComp->GetValueAsObject(BB_TargetActor) == Actor)
                {
                    BlackboardComp->ClearValue(BB_TargetActor);
                    BlackboardComp->SetValueAsBool(BB_HasLOS, false);
                }
            }
        }
    }
    if (Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
    {
        const bool bHeard = Stimulus.WasSuccessfullySensed();
        if (bHeard)
        {
            // 소리 난 위치 기록
            BlackboardComp->SetValueAsVector(BB_LastHeardLocation, Stimulus.StimulusLocation);
            BlackboardComp->SetValueAsBool(BB_InSoundRange, true);
        }
        else
        {
            // 소리가 사라진 경우 정리(선택). MaxAge 경과 시 자동으로 잊혀지기도 함
            BlackboardComp->ClearValue(BB_LastHeardLocation);
            BlackboardComp->SetValueAsBool(BB_InSoundRange, false);
        }
        return;
    }


}

void AAIEnemyController::OnPerceptionUpdated(const TArray<AActor*>&)
{
    if (!BlackboardComp) return;
    if (AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(BB_TargetActor)))
    {
        FActorPerceptionBlueprintInfo Info;
        PerceptionComp->GetActorsPerception(Target, Info);
        const FAIStimulus* Seen = Info.LastSensedStimuli.FindByPredicate(
            [](const FAIStimulus& S){ return S.WasSuccessfullySensed(); });
        if (Seen)
        {
            BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Seen->StimulusLocation);
        }
    }
}

void AAIEnemyController::OnTakeAnyDamage_Handled(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (!BlackboardComp) return;

    // 우선순위: DamageCauser(프로젝타일/무기 등) → Instigator Pawn
    AActor* Attacker = nullptr;

    if (DamageCauser && DamageCauser != DamagedActor)
    {
        // 프로젝타일이면 그 Owner/Instigator를 따라 올라가는 것도 방법
        if (APawn* CauserPawn = Cast<APawn>(DamageCauser))
            Attacker = CauserPawn;
        else if (AActor* CauserOwner = DamageCauser->GetOwner())
            Attacker = CauserOwner;
        else
            Attacker = DamageCauser;
    }

    if (!Attacker && InstigatedBy)
        Attacker = InstigatedBy->GetPawn();

    if (Attacker && Attacker != DamagedActor)
    {
        // 바로 타깃 지정(시야 없어도 추격 시작)
        BlackboardComp->SetValueAsObject(BB_TargetActor, Attacker);
        BlackboardComp->SetValueAsVector(BB_LastSeenLocation, Attacker->GetActorLocation());

        // 피격 플래그 (BT에서 우선 추격 시퀀스 선택하도록 쓰기 좋음)
        //BlackboardComp->SetValueAsBool(BB_WasDamagedRecently, true);

        // 일정 시간 뒤 자동 해제(옵션)
        GetWorld()->GetTimerManager().ClearTimer(Timer_DamagedFlagReset);
        GetWorld()->GetTimerManager().SetTimer(
            Timer_DamagedFlagReset, this, &AAIEnemyController::ClearDamagedFlag, 3.0f, false);
    }
}

void AAIEnemyController::ClearDamagedFlag()
{
    //if (BlackboardComp)
        //BlackboardComp->SetValueAsBool(BB_WasDamagedRecently, false);
}


void AAIEnemyController::SendDeadToBT()
{
    BlackboardComp->SetValueAsBool(TEXT("IsDead"), true);
}

void AAIEnemyController::SendStunToBT(bool b)
{
    BlackboardComp->SetValueAsBool(TEXT("IsStunned"), b);
}

void AAIEnemyController::ClearTarget()
{
    BlackboardComp->ClearValue(BB_TargetActor);
    BlackboardComp->SetValueAsBool(BB_HasLOS, false);
    BlackboardComp->SetValueAsBool(BB_InAttackRange, false);
    BlackboardComp->SetValueAsFloat(BB_TargetDistance, 0.f);
    // LastSeenLocation은 유지하거나, 필요 시 (0,0,0)으로 초기화
}
