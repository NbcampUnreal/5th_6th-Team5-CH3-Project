// AnimNotify_AttackExit.cpp
#include "AnimNotify_AttackExit.h"
#include "../Component/MeleeAttackComponent.h"
#include "../Component/RangedAttackComponent.h"
#include "../Component/Stage2BossAttackComponent.h"

void UAnimNotify_AttackExit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        //UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackExit start"));
        bool bHandled = false;

        if (auto* Melee = Owner->FindComponentByClass<UMeleeAttackComponent>())
        {
            Melee->FinishAttack();
            Melee->bAttackEnded = true;
            bHandled = true;
        }
        if (auto* Boss2 = Owner->FindComponentByClass<UStage2BossAttackComponent>())
        {
            Boss2->FinishAttack();
            Boss2->bAttackEnded = true;
            bHandled = true;
        }
        if (!bHandled)
        {
            if (auto* Ranged = Owner->FindComponentByClass<URangedAttackComponent>())
                Ranged->FinishAttack();
        }
    }
}
