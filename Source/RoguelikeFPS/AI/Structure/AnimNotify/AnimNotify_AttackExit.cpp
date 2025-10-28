// AnimNotify_AttackExit.cpp
#include "AnimNotify_AttackExit.h"
#include "../MeleeAttackComponent.h"
#include "../RangedAttackComponent.h"

void UAnimNotify_AttackExit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackExit start"));
        bool bHandled = false;

        if (auto* Melee = Owner->FindComponentByClass<UMeleeAttackComponent>())
        {
            Melee->FinishAttack();
            bHandled = true;
        }
        if (!bHandled)
        {
            if (auto* Ranged = Owner->FindComponentByClass<URangedAttackComponent>())
                Ranged->FinishAttack();
        }
    }
}
