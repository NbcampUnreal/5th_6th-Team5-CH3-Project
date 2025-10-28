#include "EnemyStateMachineComponent.h"
#include "AIController.h"
#include "EnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MeleeAttackComponent.h"
#include "RangedAttackComponent.h"
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


    if (AActor* Owner = GetOwner())// 공격 컴포넌트 로드
    {
        MeleeComp = Owner->FindComponentByClass<UMeleeAttackComponent>();
        RangedComp = Owner->FindComponentByClass<URangedAttackComponent>();
    }
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
    if (MeleeComp.IsValid())
        MeleeComp->OnAttackFinished.RemoveAll(this);
    if (RangedComp.IsValid())
        RangedComp->OnAttackFinished.RemoveAll(this);

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
    if (MeleeComp.IsValid())
        MeleeComp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::HandleAttackFinished);
    ChangeState(EEnemyState::Chase);
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
        UE_LOG(LogTemp, Log, TEXT("Entered Idle"));
        CachedAnim->SetAnimState(EEnemyState::Idle);
        CachedAnim->SetInAttack(false);
        CachedAnim->SetHasTarget(false);

        break;

    case EEnemyState::Patrol:
        UE_LOG(LogTemp, Log, TEXT("Entered Patrol"));
        CachedAnim->SetAnimState(EEnemyState::Patrol);
        CachedAnim->SetInAttack(false);
        CachedAnim->SetHasTarget(false);
        break;

    case EEnemyState::Chase:
        UE_LOG(LogTemp, Log, TEXT("Entered Chase"));
        CachedAnim->SetAnimState(EEnemyState::Chase);
        CachedAnim->SetInAttack(false);
        CachedAnim->SetHasTarget(true);
        break;

    case EEnemyState::Attack:
        UE_LOG(LogTemp, Log, TEXT("Entered Attack"));
        CachedAnim->SetAnimState(EEnemyState::Attack);
        CachedAnim->SetInAttack(true);
        UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (CachedAnim->bInAttack ? TEXT("true") : TEXT("false")));
        CachedAnim->SetHasTarget(true);
        UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (CachedAnim->bHasTarget ? TEXT("true") : TEXT("false")));
        OnEnter_Attack();
        break;

    case EEnemyState::Dead:
        UE_LOG(LogTemp, Log, TEXT("Entered Dead"));
        break;
    }
}

void UEnemyStateMachineComponent::OnExitState(EEnemyState State)
{
    switch (State)
    {
    case EEnemyState::Idle:
        UE_LOG(LogTemp, Log, TEXT("Exit Idle"));
        break;
    case EEnemyState::Attack:
        UE_LOG(LogTemp, Log, TEXT("Exit Attack"));
        break;
    case EEnemyState::Chase:
        UE_LOG(LogTemp, Log, TEXT("Exit Chase"));
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
        // Idle 동안 지속 처리할 로직
        break;

    case EEnemyState::Chase:
        // Chase 동안 AI가 추적 중일 때 처리
        break;

    case EEnemyState::Attack:
        // 공격 상태 유지 중
        break;
    }
}

void UEnemyStateMachineComponent::OnEnter_Attack()
{
    AActor* Target = GetTargetFromBlackboard();
    if (!Target) { ChangeState(EEnemyState::Chase); return; }

    const float Dist = Dist2DToTarget(Target);

    // 우선순위: 근접 가능하면 근접, 아니면 원거리
    if (MeleeComp.IsValid() && Dist <= MeleeThreshold && MeleeComp->CanAttack())
    {
        MeleeComp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::HandleAttackFinished);
        MeleeComp->OnAttackFinished.AddDynamic(this, &UEnemyStateMachineComponent::HandleAttackFinished);
        MeleeComp->StartAttack(Target);
    }
    else if (RangedComp.IsValid() && RangedComp->CanAttack())
    {
        RangedComp->OnAttackFinished.RemoveDynamic(this, &UEnemyStateMachineComponent::HandleAttackFinished);
        RangedComp->OnAttackFinished.AddDynamic(this, &UEnemyStateMachineComponent::HandleAttackFinished);
        RangedComp->StartAttack(Target);
    }
    else
    {
        //ChangeState(EEnemyState::Chase);
    }

}


