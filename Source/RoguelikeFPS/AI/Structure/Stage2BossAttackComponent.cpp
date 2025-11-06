#include "Stage2BossAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "EnemyConfig.h"
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
}

void UStage2BossAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);

}
    

void UStage2BossAttackComponent::DoHit()
{
    if (bCanceled) return;
    if (!HasServerAuthority()) return;
    //if (!bAttackInProgress) return;
    UE_LOG(LogTemp, Log, TEXT("DoHit start"));
    int rand = FMath::RandRange(0, 2);
    switch (rand)
    {
        case 0:
            HitShape = EHitShape::Capsule;
            break;
        case 1:
            HitShape = EHitShape::Box;
            break;
        case 2:
            HitShape = EHitShape::Sector;
            break;
        default:
            break;
    }

    TraceAndApplyDamage();
}

void UStage2BossAttackComponent::TraceAndApplyDamage()
{
    UE_LOG(LogTemp, Log, TEXT("DoHit1 start"));
    switch (HitShape)
    {
    case EHitShape::Capsule:
        DoCapsuleTraceAndDamage();  // 기존 코드 이동
        break;
    case EHitShape::Box:
        DoBoxTraceAndDamage();
        break;
    case EHitShape::Sector:
        DoSectorAndDamage();
        break;
    }
}

void UStage2BossAttackComponent::DoCapsuleTraceAndDamage()
{
    // 1) 기본 안전 체크
    const AActor* Owner = GetOwner();
    if (!Owner) return;

    UE_LOG(LogTemp, Log, TEXT("capsule start"));
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

void UStage2BossAttackComponent::DoBoxTraceAndDamage()
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;

    UE_LOG(LogTemp, Log, TEXT("box start"));

    const FRotator Rot = Owner->GetActorRotation();
    const FVector  Fwd = Rot.Vector();

    // 박스 중심: 현재 위치에서 전방으로 Offset만큼
    const FVector Center = Owner->GetActorLocation() + Fwd * BoxForwardOffset;
    const FQuat   Q = Rot.Quaternion();

    TArray<FHitResult> Hits;
    TArray<AActor*> Ignore; Ignore.Add(const_cast<AActor*>(Owner));

    UKismetSystemLibrary::BoxTraceMulti(
        this, Center, Center, BoxHalfExtent, Rot,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        false, Ignore,
        bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
        Hits, true, FLinearColor::Green, FLinearColor::Red, DebugDrawTime
    );

    TArray<AActor*> Unique;
    for (const FHitResult& H : Hits)
    {
        if (AActor* A = H.GetActor())
        {
            if (A != Owner) Unique.AddUnique(A);
        }
    }

    for (AActor* A : Unique)
    {
        UGameplayStatics::ApplyDamage(A, /*Damage*/ 10.f, nullptr,
            const_cast<AActor*>(Owner), UDamageType::StaticClass());
    }

    if (bDebug)
    {
        DrawDebugBox(GetWorld(), Center, BoxHalfExtent, Q, FColor::Green, false, DebugDrawTime, 0, 1.5f);
        DrawDebugDirectionalArrow(GetWorld(),
            Owner->GetActorLocation(),
            Owner->GetActorLocation() + Fwd * (BoxHalfExtent.Y * 2.f),
            30.f, FColor::Cyan, false, DebugDrawTime, 0, 2.f);
    }
}

void UStage2BossAttackComponent::DoSectorAndDamage()
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;

    UE_LOG(LogTemp, Log, TEXT("sector start"));

    const FVector Origin = Owner->GetActorLocation();
    const FRotator Rot = Owner->GetActorRotation();
    const FVector Fwd2D = FVector(Rot.Vector().X, Rot.Vector().Y, 0).GetSafeNormal();

    // 반지름: 기본은 Range 사용, 따로 쓰고 싶으면 SectorRadius 사용
    const float R = (SectorRadius > 0.f) ? SectorRadius : Range;
    const float HalfAngleRad = FMath::DegreesToRadians(SectorAngleDeg * 0.5f);
    const float CosThreshold = FMath::Cos(HalfAngleRad);

    // 1) 반지름 내 후보 수집
    TArray<AActor*> Overlapped;
    TArray<AActor*> Ignore; Ignore.Add(const_cast<AActor*>(Owner));

    UKismetSystemLibrary::SphereOverlapActors(
        this, Origin, R,
        { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
        nullptr, Ignore, Overlapped
    );

    // 2) 각도/높이 필터
    TArray<AActor*> Targets;
    for (AActor* A : Overlapped)
    {
        if (!IsValid(A) || A == Owner) continue;

        const FVector To = A->GetActorLocation() - Origin;

        // 높이 제한이 있으면 체크
        if (SectorHalfHeight > 0.f)
        {
            const float DZ = FMath::Abs(To.Z);
            if (DZ > SectorHalfHeight) continue;
        }

        const FVector Dir2D = FVector(To.X, To.Y, 0).GetSafeNormal();
        if (FVector::DotProduct(Dir2D, Fwd2D) >= CosThreshold)
        {
            // 거리(2D) 체크도 한번 더
            if (FVector::Dist2D(Origin, A->GetActorLocation()) <= R)
                Targets.Add(A);
        }
    }

    // 3) 데미지 적용
    for (AActor* A : Targets)
    {
        UGameplayStatics::ApplyDamage(A, /*Damage*/ 10.f, nullptr,
            const_cast<AActor*>(Owner), UDamageType::StaticClass());
    }

    // 4) 디버그 가이드
    if (bDebug)
    {
        const float Half = SectorAngleDeg * 0.5f;
        const float Step = 10.f;
        for (float a = -Half; a <= Half; a += Step)
        {
            const FRotator Rr(0, a, 0);
            const FVector P = Origin + (Rot + Rr).Vector() * R;
            DrawDebugLine(GetWorld(), Origin, P, FColor::Yellow, false, DebugDrawTime, 0, 0.5f);
        }
        // 경계선
        DrawDebugLine(GetWorld(), Origin, Origin + (Rot + FRotator(0, -Half, 0)).Vector() * R, FColor::Red, false, DebugDrawTime, 0, 1.5f);
        DrawDebugLine(GetWorld(), Origin, Origin + (Rot + FRotator(0, Half, 0)).Vector() * R, FColor::Red, false, DebugDrawTime, 0, 1.5f);

        if (SectorHalfHeight > 0.f)
        {
            DrawDebugBox(GetWorld(), Origin, FVector(10, R, SectorHalfHeight),
                FRotationMatrix::MakeFromX(Rot.Vector()).ToQuat(), FColor::Cyan, false, DebugDrawTime, 0, 0.5f);
        }
    }
}

