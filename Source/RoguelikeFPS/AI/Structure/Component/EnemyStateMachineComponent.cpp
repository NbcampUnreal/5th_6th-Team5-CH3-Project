// EnemyStateMachineComponent.cpp
#include "EnemyStateMachineComponent.h"
#include "AIController.h"
#include "../EnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MeleeAttackComponent.h"
#include "RangedAttackComponent.h"
#include "Stage2BossAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

UEnemyStateMachineComponent::UEnemyStateMachineComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CurrentState = EEnemyState::Idle;
}

void UEnemyStateMachineComponent::BeginPlay()
{
    Super::BeginPlay();

    // 공격 컴포넌트 로드
    if (AActor* Owner = GetOwner())
    {
        MeleeComp = Owner->FindComponentByClass<UMeleeAttackComponent>();
        RangedComp = Owner->FindComponentByClass<URangedAttackComponent>();
        Boss2Comp = Owner->FindComponentByClass<UStage2BossAttackComponent>();
    }

    // 애님 인스턴스 캐시
    if (const ACharacter* C = Cast<ACharacter>(GetOwner()))
    {
        if (USkeletalMeshComponent* Mesh = C->GetMesh())
        {
            if (UAnimInstance* AI = Mesh->GetAnimInstance())
            {
                CachedAnim = Cast<UEnemyAnimInstance>(AI);
            }
        }
    }

    OnEnterState(CurrentState); // 시작 상태 진입
}

void UEnemyStateMachineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 모든 델리게이트 정리(중복 호출 방지)
    if (MeleeComp.IsValid())
        MeleeComp->OnAttackFinished.RemoveAll(this);
    if (RangedComp.IsValid())
        RangedComp->OnAttackFinished.RemoveAll(this);
    if (Boss2Comp.IsValid())
        Boss2Comp->OnAttackFinished.RemoveAll(this);

    Super::EndPlay(EndPlayReason);
}

void UEnemyStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TickState(CurrentState, DeltaTime);
}

void UEnemyStateMachineComponent::ChangeState(EEnemyState NewState)
{
    if (NewState == CurrentState) return;

    OnExitState(CurrentState);
    CurrentState = NewState;
    OnEnterState(NewState);
}

AActor* UEnemyStateMachineComponent::GetTargetFromBlackboard() const
{
    const APawn* Pawn = Cast<APawn>(GetOwner());
    if (!Pawn) return nullptr;

    const AAIController* AICon = Cast<AAIController>(Pawn->GetController());
    if (!AICon) return nullptr;

    if (const UBlackboardComponent* BB = AICon->GetBlackboardComponent())
        return Cast<AActor>(BB->GetValueAsObject(BBKey_TargetActor));

    return nullptr;
}

void UEnemyStateMachineComponent::HandleAttackFinished()
{
    // 하위 호환용 — 내부에서 공통 마무리 호출
    UE_LOG(LogTemp, Verbose, TEXT("HandleAttackFinished() called (compat)."));
    FinalizeAfterAttack();
}

void UEnemyStateMachineComponent::SetCombatLocked(bool bLocked)
{
    if (const APawn* P = Cast<APawn>(GetOwner()))
        if (AAIController* AC = Cast<AAIController>(P->GetController()))
            if (UBlackboardComponent* BB = AC->GetBlackboardComponent())
                BB->SetValueAsBool(TEXT("IsCombatLocked"), bLocked);
}

float UEnemyStateMachineComponent::Dist2DToTarget(AActor* Target) const
{
    if (!Target) return TNumericLimits<float>::Max();
    const AActor* Owner = GetOwner();
    return FVector::Dist2D(Owner->GetActorLocation(), Target->GetActorLocation());
}

void UEnemyStateMachineComponent::OnEnterState(EEnemyState State)
{
    switch (State)
    {
    case EEnemyState::Idle:
        if (CachedAnim)
        {
            //UE_LOG(LogTemp, Log, TEXT("Enter Idle"));
            CachedAnim->SetAnimState(EEnemyState::Idle);
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(false);
        }
        break;

    case EEnemyState::Patrol:
        if (CachedAnim)
        {
            CachedAnim->SetAnimState(EEnemyState::Patrol);
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(false);
        }
        break;

    case EEnemyState::Chase:
        if (CachedAnim)
        {
            //UE_LOG(LogTemp, Log, TEXT("Enter Chase"));
            SetCombatLocked(false);
            CachedAnim->SetAnimState(EEnemyState::Chase);
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(true);
        }
        break;

    case EEnemyState::Stun:
        if (CachedAnim)
        {
            CachedAnim->SetAnimState(EEnemyState::Stun);
            CachedAnim->SetIsStun(true);
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(false);
        }
        break;

    case EEnemyState::Attack:
        if (CachedAnim)
        {
            //UE_LOG(LogTemp, Log, TEXT("Enter Attack"));
            CachedAnim->SetAnimState(EEnemyState::Attack);
            CachedAnim->SetInAttack(true);
            CachedAnim->SetHasTarget(true);
        }
        OnEnter_Attack();
        break;

    case EEnemyState::Recover:
        if (CachedAnim)
        {
            // 공격 종료 직후의 리커버리 상태 – 이동은 할 수도/안 할 수도
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(GetTargetFromBlackboard() != nullptr);
            CachedAnim->SetAnimState(EEnemyState::Recover); // 필요 없으면 생략 가능(Chase로 보이게 하고 싶다면 설정하지 않음)
        }

        // 짧은 대기 후 다음 행동 결정
        GetWorld()->GetTimerManager().SetTimer(
            PostAttackTimer, this,
            &UEnemyStateMachineComponent::OnPostAttackDelayElapsed,
            PostAttackDelay, false);
        break;

    case EEnemyState::Dead:
        if (CachedAnim)
        {
            CachedAnim->SetAnimState(EEnemyState::Dead);
            CachedAnim->SetInAttack(false);
            CachedAnim->SetHasTarget(false);
            CachedAnim->SetIsDead(true);
        }
        break;
    }
}

void UEnemyStateMachineComponent::OnExitState(EEnemyState State)
{


    switch (State)
    {
    case EEnemyState::Attack:
        if (CachedAnim) CachedAnim->SetInAttack(false);
        SetCombatLocked(false);
        break;

    case EEnemyState::Stun:
        if (CachedAnim)
            CachedAnim->SetIsStun(false);
        break;

    case EEnemyState::Recover:
        GetWorld()->GetTimerManager().ClearTimer(PostAttackTimer);
        break;

    default:
        break;
    }
}

void UEnemyStateMachineComponent::TickState(EEnemyState State, float DeltaTime)
{
    switch (State)
    {
    case EEnemyState::Idle:
        break;
    case EEnemyState::Chase:
        break;
    case EEnemyState::Attack:
        break;
    default:
        break;
    }
}

void UEnemyStateMachineComponent::OnEnter_Attack()
{
    AActor* Target = GetTargetFromBlackboard();
    if (!Target) 
    { 
        ChangeState(EEnemyState::Chase); 
        return; 
    }

    if (CachedAnim) CachedAnim->SetInAttack(true);
    SetCombatLocked(true);

    const float Dist = Dist2DToTarget(Target);

    // 직전 패턴과 다른 패턴 선택
    EPattern NowPattern = EPattern::Pattern1;
    if (CachedAnim)
    {
        while (true)
        {
            const int Rand = FMath::RandRange(0, 2);
            NowPattern = (Rand == 0) ? EPattern::Pattern1 :
                (Rand == 1) ? EPattern::Pattern2 :
                EPattern::Pattern3;
            if (NowPattern != CachedAnim->PatternState) break;
        }

        CachedAnim->SetPattern(NowPattern);
    }

    // 바인딩 전 기존 바인딩 제거(중복 방지)
    if (MeleeComp.IsValid())
        MeleeComp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::OnMeleeFinished);
    if (RangedComp.IsValid())
        RangedComp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::OnRangedFinished);
    if (Boss2Comp.IsValid())
        Boss2Comp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::OnBoss2Finished);

    if (MeleeComp.IsValid() && MeleeComp->CanAttack())
    {
        MeleeThreshold = MeleeComp->GetAttackRange();
        if (Dist <= MeleeThreshold)
        {
            MeleeComp->OnAttackFinished.AddDynamic(this, &UEnemyStateMachineComponent::OnMeleeFinished);
            MeleeComp->StartAttack(Target);
            return;
        }
    }

    if (RangedComp.IsValid() && RangedComp->CanAttack())
    {
        RangedComp->OnAttackFinished.AddDynamic(this, &UEnemyStateMachineComponent::OnRangedFinished);
        RangedComp->StartAttack(Target);
        return;
    }

    if (Boss2Comp.IsValid() && Boss2Comp->CanAttack())
    {
        // 보스2는 패턴 번호를 시작 전에 확정
        if (CachedAnim)
        {
            switch (CachedAnim->PatternState)
            {
            case EPattern::Pattern1: Boss2Comp->patternNum = 1; break;
            case EPattern::Pattern2: Boss2Comp->patternNum = 2; break;
            case EPattern::Pattern3: Boss2Comp->patternNum = 3; break;
            default:                  Boss2Comp->patternNum = 1; break;
            }
        }

        MeleeThreshold = Boss2Comp->GetAttackRange();
        if (Dist <= MeleeThreshold)
        {
            Boss2Comp->OnAttackFinished.AddDynamic(this, &UEnemyStateMachineComponent::OnBoss2Finished);
            Boss2Comp->StartAttack(Target);
            return;
        }
    }

    // 여기까지 못했으면 추격으로
    ChangeState(EEnemyState::Chase);
}

void UEnemyStateMachineComponent::OnMeleeFinished()
{
    UE_LOG(LogTemp, Log, TEXT("OnMeleeFinished"));
    FinalizeAfterAttack();
}

void UEnemyStateMachineComponent::OnRangedFinished()
{
    UE_LOG(LogTemp, Log, TEXT("OnRangedFinished"));
    FinalizeAfterAttack();
}

void UEnemyStateMachineComponent::OnBoss2Finished()
{
    UE_LOG(LogTemp, Log, TEXT("OnBoss2Finished"));
    FinalizeAfterAttack();
}

void UEnemyStateMachineComponent::FinalizeAfterAttack()
{
    if (MeleeComp.IsValid())  MeleeComp->OnAttackFinished.RemoveAll(this);
    if (RangedComp.IsValid()) RangedComp->OnAttackFinished.RemoveAll(this);
    if (Boss2Comp.IsValid())  Boss2Comp->OnAttackFinished.RemoveAll(this);

    if (CachedAnim) CachedAnim->SetInAttack(false);
    SetCombatLocked(false);

    AActor* Target = GetTargetFromBlackboard();
    if (!Target)
    {
        if (CachedAnim) CachedAnim->SetInAttack(false);
        ChangeState(EEnemyState::Idle);
        return;
    }

    const float Dist = Dist2DToTarget(Target);
    const bool bMeleeReady = MeleeComp.IsValid() && MeleeComp->CanAttack() && Dist <= MeleeComp->GetAttackRange();
    const bool bBoss2Ready = Boss2Comp.IsValid() && Boss2Comp->CanAttack() && Dist <= Boss2Comp->GetAttackRange();
    const bool bRangeReady = RangedComp.IsValid() && RangedComp->CanAttack(); // 원거리 범위 체크는 의도대로 유지

    const bool bAnyReady = (bMeleeReady || bBoss2Ready || bRangeReady);

    // 사거리 안에 "있나?" (원거리까지 통일하고 싶으면 조건에 포함)
    const bool bInAnyAttackRange =
        (MeleeComp.IsValid() && Dist <= MeleeComp->GetAttackRange()) ||
        (Boss2Comp.IsValid() && Dist <= Boss2Comp->GetAttackRange()) ||
        (RangedComp.IsValid()); // 원거리는 사거리 제한이 없다면 항상 true 취급(의도에 맞게 조정)

    if (bAnyReady)
    {
        // 바로 재공격
        if (CachedAnim) CachedAnim->SetInAttack(true);
        SetCombatLocked(true);
        OnEnter_Attack();
        return;
    }

    // 준비는 안됐지만 사거리 안이라면 상태 유지 + 짧은 타이머로 재검사
    if (bInAnyAttackRange)
    {
        if (CachedAnim) CachedAnim->SetInAttack(true); // BT가 다른 브랜치로 안 새도록 유지
        // 이미 돌고 있지 않다면 0.1초 간격으로 재검사
        if (!GetWorld()->GetTimerManager().IsTimerActive(AttackRecheckTimer))
        {
            GetWorld()->GetTimerManager().SetTimer(
                AttackRecheckTimer, this, &UEnemyStateMachineComponent::RecheckAttackReady, 0.1f, true);
        }
        return;
    }

    // 여기까지 왔으면 사거리 밖: 타이머 정리하고 Chase로
    if (CachedAnim) CachedAnim->SetInAttack(false);
    GetWorld()->GetTimerManager().ClearTimer(AttackRecheckTimer);
    ChangeState(EEnemyState::Chase);
}

void UEnemyStateMachineComponent::RecheckAttackReady()
{
    AActor* Target = GetTargetFromBlackboard();
    if (!Target)
    {
        if (CachedAnim) CachedAnim->SetInAttack(false);
        GetWorld()->GetTimerManager().ClearTimer(AttackRecheckTimer);
        ChangeState(EEnemyState::Idle);
        return;
    }

    const float Dist = Dist2DToTarget(Target);
    const bool bMeleeReady = MeleeComp.IsValid() && MeleeComp->CanAttack() && Dist <= MeleeComp->GetAttackRange();
    const bool bBoss2Ready = Boss2Comp.IsValid() && Boss2Comp->CanAttack() && Dist <= Boss2Comp->GetAttackRange();
    const bool bRangeReady = RangedComp.IsValid() && RangedComp->CanAttack();

    if (bMeleeReady || bBoss2Ready || bRangeReady)
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackRecheckTimer);
        if (CachedAnim) CachedAnim->SetInAttack(true);
        OnEnter_Attack();
        return;
    }

    // 사거리 밖으로 나갔으면 Chase로 전환
    const bool bInAnyAttackRange =
        (MeleeComp.IsValid() && Dist <= MeleeComp->GetAttackRange()) ||
        (Boss2Comp.IsValid() && Dist <= Boss2Comp->GetAttackRange()) ||
        (RangedComp.IsValid());

    if (!bInAnyAttackRange)
    {
        if (CachedAnim) CachedAnim->SetInAttack(false);
        GetWorld()->GetTimerManager().ClearTimer(AttackRecheckTimer);
        ChangeState(EEnemyState::Chase);
    }
}



void UEnemyStateMachineComponent::OnPostAttackDelayElapsed()
{
    AActor* Target = GetTargetFromBlackboard();
    if (!Target)
    {
        ChangeState(EEnemyState::Idle);
        return;
    }

    const float Dist = Dist2DToTarget(Target);

    // 우선순위: 근접 → 원거리 → 보스2 (프로젝트 규칙대로)
    if (MeleeComp.IsValid() && MeleeComp->CanAttack() && Dist <= MeleeComp->GetAttackRange())
    {
        ChangeState(EEnemyState::Attack);
        return;
    }

    if (Boss2Comp.IsValid() && Boss2Comp->CanAttack() && Dist <= Boss2Comp->GetAttackRange())
    {
        ChangeState(EEnemyState::Attack);
        return;
    }

    if (RangedComp.IsValid() && RangedComp->CanAttack())
    {
        ChangeState(EEnemyState::Attack);
        return;
    }

    // 아직 공격 타이머/쿨타임이거나 사거리 밖이면 추격
    ChangeState(EEnemyState::Chase);
}