#include "RangedAttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnemyConfig.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Animation/AnimInstance.h"

void URangedAttackComponent::ApplyRangeConfig(const UEnemyConfig* Config)
{
    if (!Config) return;
    ApplyConfig(Config);            // 공통(Damage/Cooldown)
    ProjectileClass = Config->ProjectileClass;
    MuzzleSocket = Config->MuzzleSocket;
    MuzzleSpeed = Config->ProjectileSpeed;
    
}

void URangedAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);
    //UE_LOG(LogTemp, Log, TEXT("Entered RangedStartAttack"));
    //if (!CanAttack()) return;

    //if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    //{
    //    if (AttackMontage && OwnerChar->GetMesh() && OwnerChar->GetMesh()->GetAnimInstance())
    //    {
    //        OwnerChar->GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &URangedAttackComponent::OnMontageEnded);
    //        OwnerChar->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &URangedAttackComponent::OnMontageEnded);
    //        OwnerChar->PlayAnimMontage(AttackMontage);
    //    }
    //    else
    //    {
    //        DoHit();
    //        FinishAttack();
    //    }
    //}
}

void URangedAttackComponent::DoHit()
{
    if (bCanceled) return;
    if (!HasServerAuthority()) return;
    if (!bAttackInProgress) return;
    if (!ProjectileClass) return;

    FVector Loc; FRotator Rot;
    GetMuzzleTransform(Loc, Rot);

    FActorSpawnParameters P;
    P.Owner = GetOwner();
    P.Instigator = Cast<APawn>(GetOwner());

    if (AActor* Proj = GetWorld()->SpawnActor<AActor>(ProjectileClass, Loc, Rot, P))
    {
        if (auto* Move = Proj->FindComponentByClass<UProjectileMovementComponent>())
        {
            Move->InitialSpeed = Move->MaxSpeed = MuzzleSpeed;
        }
    }
}

void URangedAttackComponent::GetMuzzleTransform(FVector& OutLoc, FRotator& OutRot) const
{
    OutLoc = GetOwner()->GetActorLocation();
    OutRot = GetOwner()->GetActorRotation();

    if (const ACharacter* C = Cast<ACharacter>(GetOwner()))
    {
        if (const USkeletalMeshComponent* Mesh = C->GetMesh())
        {
            OutLoc = Mesh->GetSocketLocation(MuzzleSocket);
            if (CurrentTarget.IsValid())
                OutRot = (CurrentTarget->GetActorLocation() - OutLoc).Rotation();
            else
                OutRot = Mesh->GetSocketRotation(MuzzleSocket);
        }
    }
}