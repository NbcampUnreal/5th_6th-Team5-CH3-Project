#include "MeleeAttackComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyConfig.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"

void UMeleeAttackComponent::ApplyMeleeConfig(const UEnemyConfig* Config)
{
    if (!Config) return;
    ApplyConfig(Config);            // 공통(Damage/Cooldown)
    Range = Config->Range;
    Radius = Config->AttackRange;
}

void UMeleeAttackComponent::StartAttack(AActor* Target)
{
    Super::StartAttack(Target);
    //UE_LOG(LogTemp, Log, TEXT("Entered MeleeStartAttack"));
    //if (bAttackInProgress || !CanAttack()) return;
    //bAttackInProgress = true;

    //if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    //{
    //    if (UAnimInstance* Anim = OwnerChar->GetMesh()->GetAnimInstance())
    //    {
    //        Anim->OnMontageEnded.RemoveDynamic(this, &UMeleeAttackComponent::OnMontageEnded);
    //        Anim->OnMontageEnded.AddUniqueDynamic(this, &UMeleeAttackComponent::OnMontageEnded);

    //        if (AttackMontage) OwnerChar->PlayAnimMontage(AttackMontage);
    //        else { DoHit(); FinishAttack(); } // 몽타주 없으면 즉시형
    //    }
    //}
        
}

void UMeleeAttackComponent::DoHit()
{
    if (bCanceled) return;
    if (!HasServerAuthority()) return;
    if (!bAttackInProgress) return;

    TraceAndApplyDamage();
}

void UMeleeAttackComponent::TraceAndApplyDamage()
{
    const AActor* Owner = GetOwner();
    if (!Owner) return;

    const FVector Start = Owner->GetActorLocation();
    const FVector End = Start + Owner->GetActorForwardVector() * Range;

    TArray<FHitResult> Hits;
    UKismetSystemLibrary::SphereTraceMulti(
        this, Start, End, Radius,
        UEngineTypes::ConvertToTraceType(ECC_Pawn),
        false, { const_cast<AActor*>(Owner) }, EDrawDebugTrace::None, Hits, true);

    for (const FHitResult& H : Hits)
    {
        if (AActor* HitActor = H.GetActor())
        {
            if (HitActor != Owner)
            {
                const bool bIsPlayer = HitActor->ActorHasTag(!TEXT("Enemy"));
                if (bIsPlayer)
                {
                    UGameplayStatics::ApplyDamage(HitActor, Damage, nullptr, const_cast<AActor*>(Owner), UDamageType::StaticClass());
                }
                    
            }
                
        }
    }
}