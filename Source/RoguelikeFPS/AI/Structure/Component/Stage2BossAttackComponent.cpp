#include "Stage2BossAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../EnemyConfig.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

void UStage2BossAttackComponent::ApplyMeleeConfig(const UEnemyConfig* Config, float ConfigDamage)
{
    if (!Config) return;
    ApplyConfig(Config, ConfigDamage);            // 공통(Damage/Cooldown)
    Range = Config->MeleeRange;        // 공격 길이
    Radius = Config->MeleeHitRadius;   // 공격 두께
    AttackRange = Config->AttackRange;   // 공격 가능 판정
    AddtionalRange = Config->AddtionalRange;
}

void UStage2BossAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);

}
    

void UStage2BossAttackComponent::DoHit()
{
    //if (bCanceled) return;
    if (!HasServerAuthority()) return;
    //if (!bAttackInProgress) return;
    UE_LOG(LogTemp, Log, TEXT("DoHit start"));
    TraceAndApplyDamage();
}

void UStage2BossAttackComponent::TraceAndApplyDamage()
{
    UE_LOG(LogTemp, Log, TEXT("pattern num %d"), patternNum);
    DoCapsuleTraceAndDamage();
}

void UStage2BossAttackComponent::DoCapsuleTraceAndDamage()
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // Config에서 복사된 멤버 값 사용
    const FVector Start = Owner->GetActorLocation();

    const FVector Vel = Owner->GetVelocity();
    const bool bHasVelocity = !Vel.IsNearlyZero(1e-3f);
    const FVector Fwd = bHasVelocity ? Vel.GetSafeNormal() : Owner->GetActorForwardVector();

    const FVector End = Start + Fwd * (Range + AddtionalRange) ;
    const FVector AttackRangeEnd = Start + Owner->GetActorForwardVector() * AttackRange;

    const float Dist = (End - Start).Size();
    const bool  bDoSweep = Dist > KINDA_SMALL_NUMBER;

    FCollisionQueryParams Params(SCENE_QUERY_STAT(Stage2BossMelee), bTraceComplex);
    Params.bReturnPhysicalMaterial = false;
    Params.AddIgnoredActor(Owner);

    const FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);

    TArray<FHitResult> Hits;
    Hits.Reserve(8);

    bool bHitAny = false;
    if (bDoSweep)
    {
        bHitAny = World->SweepMultiByChannel(
            Hits, Start, End, FQuat::Identity, MeleeTraceChannel, SphereShape, Params);
    }

    // 디버그(선택)
    if (bDebugMeleeTrace)
    {
        if (bDoSweep)
        {
            const FVector Dir = (End - Start);
            const float HalfHeight = Dir.Size() * 0.5f;
            const FVector Center = Start + Dir * 0.5f;
            const FQuat Rot = FRotationMatrix::MakeFromZ(Dir.GetSafeNormal()).ToQuat();
            DrawDebugCapsule(World, Center, HalfHeight, Radius, Rot, FColor::Green, false, DebugDrawTime, 0, 1.5f);
        }
        else
        {
            DrawDebugSphere(World, Start, Radius, 20, FColor::Green, false, DebugDrawTime, 0, 1.5f);
        }

        const FVector Dir1 = (AttackRangeEnd - Start);
        const float HalfHeight1 = Dir1.Size() * 0.5f;
        if (HalfHeight1 > KINDA_SMALL_NUMBER)
        {
            const FVector Center1 = Start + Dir1 * 0.5f;
            const FQuat Rot1 = FRotationMatrix::MakeFromZ(Dir1.GetSafeNormal()).ToQuat();
            DrawDebugCapsule(World, Center1, HalfHeight1, Radius, Rot1, FColor::Red, false, DebugDrawTime, 0, 1.5f);
        }
    }

    if (!bHitAny || Hits.Num() == 0)
    {
        if (bAttackEnded) { HitActorsThisSwing.Reset(); bAttackEnded = false; }
        return;
    }

    // 히트 처리
    for (const FHitResult& H : Hits)
    {
        AActor* HitActor = H.GetActor();
        if (!IsValid(HitActor) || HitActor == Owner) continue;

        // 플레이어만: Enemy 태그는 제외
        if (HitActor->ActorHasTag(TEXT("Enemy"))) continue;

        if (HitActorsThisSwing.Contains(HitActor)) continue;

        if (bUseLineOfSightCheck)
        {
            FHitResult LOSHit;
            const bool bBlocked = World->LineTraceSingleByChannel(
                LOSHit, Start, HitActor->GetActorLocation(), MeleeTraceChannel, Params);
            if (bBlocked && LOSHit.GetActor() != HitActor) continue;
        }

        UGameplayStatics::ApplyDamage(
            HitActor, Damage, nullptr, const_cast<AActor*>(Owner), UDamageType::StaticClass());

        HitActorsThisSwing.Add(HitActor);
    }

    if (bAttackEnded)
    {
        HitActorsThisSwing.Reset();
        bAttackEnded = false;
    }
}
