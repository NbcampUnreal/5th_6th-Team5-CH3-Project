#include "EnemyAttackBaseComponent.h"
#include "EnemyConfig.h"
#include "EnemyStatsComponent.h"
#include "GameFramework/Actor.h"

void UEnemyAttackBaseComponent::ApplyConfig(const UEnemyConfig* Config, float ConfigDamage)
{
    if (!Config) return;
    Damage = ConfigDamage;
    Cooldown = Config->AttackCooldown;
}


bool UEnemyAttackBaseComponent::CanAttack() const
{
    const UWorld* W = GetWorld();
    return W ? (W->GetTimeSeconds() >= NextReadyTime) : false;
}

void UEnemyAttackBaseComponent::StartAttack(AActor* Target)
{
    if (!CanAttack()) return;
    //bAttackInProgress = true;
    //Damage = GetCurrentDamage();
    //UE_LOG(LogTemp, Log, TEXT("damage %f"), Damage);
    CurrentTarget = Target;
    bCanceled = false;
    // 파생 클래스에서 몽타주 재생/사운드/예열 등 구현
}

void UEnemyAttackBaseComponent::DoHit()
{
    // 파생 클래스에서 구현(트레이스/프로젝타일/ApplyDamage 등)
}

void UEnemyAttackBaseComponent::CancelAttack()
{
    bCanceled = true;
}

void UEnemyAttackBaseComponent::FinishAttack()
{
    //if (!bAttackInProgress) return; // 중복 방지
    //bAttackInProgress = false;
    MarkCooldown();
    OnAttackFinished.Broadcast();
}



void UEnemyAttackBaseComponent::MarkCooldown()
{
    if (UWorld* W = GetWorld())
        NextReadyTime = W->GetTimeSeconds() + Cooldown;
}

bool UEnemyAttackBaseComponent::HasServerAuthority() const
{
    const AActor* Owner = GetOwner();
    return Owner && Owner->HasAuthority();
}
