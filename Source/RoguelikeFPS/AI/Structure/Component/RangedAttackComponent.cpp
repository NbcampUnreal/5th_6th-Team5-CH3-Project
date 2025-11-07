#include "RangedAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../EnemyConfig.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Weapon/Projectile.h"


void URangedAttackComponent::ApplyRangeConfig(const UEnemyConfig* Config, float ConfigDamage)
{
    if (!Config) return;
    ApplyConfig(Config, ConfigDamage);            // 공통(Damage/Cooldown)
    ProjectileClass = Config->ProjectileClass;
    MuzzleSocket = Config->MuzzleSocket;
    MuzzleSpeed = Config->ProjectileSpeed;
    CorrectX = Config->CorrectX;
    CorrectY = Config->CorrectY;
    CorrectZ = Config->CorrectZ;
}

void URangedAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);

}

void URangedAttackComponent::DoHit()
{
    if (bCanceled) return;
    if (!HasServerAuthority()) return;
    if (!ProjectileClass) return;

    FVector Loc; FRotator Rot;
    GetMuzzleTransform(Loc, Rot);

    FActorSpawnParameters P;
    P.Owner = GetOwner();
    P.Instigator = Cast<APawn>(GetOwner());

    Loc = FVector(Loc.X + CorrectX, Loc.Y + CorrectY, Loc.Z + CorrectZ);

    if (AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Loc, Rot, P))
    {
        UE_LOG(LogTemp, Log, TEXT("SpawnActor if"));
        Proj->SetMovementSpeed(MuzzleSpeed);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("SpawnActor else"));
    }
}

void URangedAttackComponent::GetMuzzleTransform(FVector& OutLoc, FRotator& OutRot) const
{
    OutLoc = GetOwner()->GetActorLocation();
    OutRot = GetOwner()->GetActorRotation();

    //OutLoc = FVector(OutLoc.X + CorrectX, OutLoc.Y + CorrectY, OutLoc.Z + CorrectZ);

    if (const ACharacter* C = Cast<ACharacter>(GetOwner()))
    {
        if (const USkeletalMeshComponent* Mesh = C->GetMesh())
        {
            // 2) 소켓에서 총구 위치/회전 가져오기
            OutLoc = Mesh->GetSocketLocation(MuzzleSocket);
            OutRot = Mesh->GetSocketRotation(MuzzleSocket);

            // 3) 타겟이 있으면 타겟을 향하게 회전 보정
            if (CurrentTarget.IsValid())
                OutRot = (CurrentTarget->GetActorLocation() - OutLoc).Rotation();

            // 4) ★ 위치 보정 적용 (가장 중요한 부분)
            // 총구 회전 기준으로 앞/옆/위 방향으로 CorrectX/Y/Z만큼 이동
            const FVector Forward = OutRot.Vector();
            const FVector Right = FRotationMatrix(OutRot).GetUnitAxis(EAxis::Y);
            const FVector Up = FRotationMatrix(OutRot).GetUnitAxis(EAxis::Z);

            OutLoc += Forward * CorrectX;
            OutLoc += Right * CorrectY;
            OutLoc += Up * CorrectZ;
        }
    }
}