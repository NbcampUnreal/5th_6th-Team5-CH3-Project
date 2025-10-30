#include "MeleeAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "EnemyConfig.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

void UMeleeAttackComponent::ApplyMeleeConfig(const UEnemyConfig* Config)
{
    if (!Config) return;
    ApplyConfig(Config);            // 공통(Damage/Cooldown)
    Range = Config->MeleeRange;        // 공격 길이
    Radius = Config->MeleeHitRadius;   // 공격 두께
    AttackRange = Config->AttackRange;   // 공격 가능 판정
}

void UMeleeAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);
      
}

void UMeleeAttackComponent::DoHit()
{
    if (bCanceled) return;
    if (!HasServerAuthority()) return;
    //if (!bAttackInProgress) return;
    //UE_LOG(LogTemp, Log, TEXT("DoHit start"));
    TraceAndApplyDamage();
}

void UMeleeAttackComponent::TraceAndApplyDamage()
{
    // 1) 기본 안전 체크
    const AActor* Owner = GetOwner();
    if (!Owner) return;


    // 2) 스윕 시작/끝 지점 계산
    //    - 시작은 소유 액터 위치
    //    - 끝은 '정면 방향'으로 Range 만큼 떨어진 위치
    const FVector Start = Owner->GetActorLocation();
    const FVector Velocity = Owner->GetVelocity().GetSafeNormal();
    const FVector End = Start + Velocity * Range;
    const FVector AttackRangeEnd = Start + Owner->GetActorForwardVector() * AttackRange;

    // 3) 스피어 스윕(멀티 히트) 수행
    //    - Start→End 사이를 반지름 Radius인 구체로 훑어서 Pawn 채널에 걸리는 액터를 수집
    //    - ActorsToIgnore에 Owner를 넣어 자기 자신은 무시
    TArray<FHitResult> Hits;
    {
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(const_cast<AActor*>(Owner));

        // 간단 버전: Kismet의 디버그 드로잉 옵션을 사용(ForDuration)
        //   - 색상: TraceColor(노란색), TraceHitColor(빨간색)
        //   - DrawTime: DebugDrawTime 동안 유지
        const EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
            //? EDrawDebugTrace::ForDuration
            //: EDrawDebugTrace::None;

        UKismetSystemLibrary::SphereTraceMulti(
            this,
            Start, End,
            Radius,
            UEngineTypes::ConvertToTraceType(ECC_Pawn),
            false,                     // bTraceComplex
            ActorsToIgnore,
            DebugType,
            Hits,
            false

        );
    }

    // 4) (선택) 직접 캡슐/라인/스피어로 '스윕 볼륨' 시각화
    //    - Kismet 디버그로 충분하면 아래 블록은 꺼도 됨
    if (bDebugMeleeTrace)
    {
        const FVector Dir = (End - Start); // 공격 범위 표기용
        const float   Dist = Dir.Size();

        const FVector Dir1 = (AttackRangeEnd - Start); // 공격 사거리 표기용
        const float   Dist1 = Dir1.Size();
        if (Dist > KINDA_SMALL_NUMBER)
        {
            const FVector Center = Start + Dir * 0.5f; // 공격 범위 표기용
            const FQuat   Rot = FRotationMatrix::MakeFromZ(Dir.GetSafeNormal()).ToQuat();
            const float   HalfHeight = Dist * 0.5f;

            const FVector Center1 = Start + Dir1 * 0.5f; // 공격 사거리 표기용
            const FQuat   Rot1 = FRotationMatrix::MakeFromZ(Dir1.GetSafeNormal()).ToQuat();
            const float   HalfHeight1 = Dist1 * 0.5f;

            DrawDebugCapsule(GetWorld(), Center, HalfHeight, Radius, Rot, FColor::Green, false, DebugDrawTime); // 공격 범위 표기용
            DrawDebugCapsule(GetWorld(), Center1, HalfHeight1, Radius, Rot1, FColor::Red, false, DebugDrawTime); // 공격 사거리 표기용
        }
    }

    // 5) 히트 처리
    //    - Enemy 태그를 가진 것은 스킵(= 플레이어만 타겟)
    //    - 맞은 액터에 데미지 가함
    static TSet<AActor*> AlreadyHitActors;

    for (const FHitResult& H : Hits)
    {
        if (AActor* HitActor = H.GetActor())
        {
            if (AlreadyHitActors.Contains(HitActor)) continue;
            if (HitActor != Owner)
            {

                const bool bIsPlayer = !HitActor->ActorHasTag(TEXT("Enemy"));
                if (bIsPlayer)
                {
                    UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, const_cast<AActor*>(Owner), UDamageType::StaticClass());
                    AlreadyHitActors.Add(HitActor);
                }
                    
            }
                
        }
    }
    if (bAttackEnded) // <- AnimNotify_End 같은 시점에서 true로 설정
    {
        AlreadyHitActors.Empty();
        bAttackEnded = false;
    }
}