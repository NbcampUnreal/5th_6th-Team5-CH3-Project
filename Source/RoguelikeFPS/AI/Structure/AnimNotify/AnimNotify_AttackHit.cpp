// AnimNotify_AttackHit.cpp
#include "AnimNotify_AttackHit.h"
#include "../Component/MeleeAttackComponent.h"
#include "../Component/RangedAttackComponent.h"
#include "../Component/Stage2BossAttackComponent.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit start"));
        if (auto* Melee = Owner->FindComponentByClass<UMeleeAttackComponent>())
        {
            UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit melee start"));
            Melee->DoHit();
        }

        else if (auto* Boss2 = Owner->FindComponentByClass<UStage2BossAttackComponent>())
        {
            UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit boss2 start"));
            Boss2->DoHit();
        }

        else if (auto* Ranged = Owner->FindComponentByClass<URangedAttackComponent>())
        {
            UE_LOG(LogTemp, Log, TEXT("Ranged start"));
            Ranged->DoHit();
        }
            
    }
}
