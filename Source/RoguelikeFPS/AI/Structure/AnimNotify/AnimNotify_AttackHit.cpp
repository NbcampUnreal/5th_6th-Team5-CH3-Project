// AnimNotify_AttackHit.cpp
#include "AnimNotify_AttackHit.h"
#include "../MeleeAttackComponent.h"
#include "../RangedAttackComponent.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit start"));
        if (auto* Melee = Owner->FindComponentByClass<UMeleeAttackComponent>())
            Melee->DoHit();
        else if (auto* Ranged = Owner->FindComponentByClass<URangedAttackComponent>())
            Ranged->DoHit();
    }
}
