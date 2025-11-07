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
        UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackExit"));

        if (auto* Melee = Owner->FindComponentByClass<UMeleeAttackComponent>())
        {
            Melee->FinishAttack();
            Melee->bAttackEnded = true;
            return;
        }
        if (auto* Boss2 = Owner->FindComponentByClass<UStage2BossAttackComponent>())
        {
            Boss2->FinishAttack();
            Boss2->bAttackEnded = true;
            return;
        }
        if (auto* Ranged = Owner->FindComponentByClass<URangedAttackComponent>())
        {
            Ranged->FinishAttack();
            return;
        }

        // FinishAttack() 내부에서 OnAttackFinished 브로드캐스트 → 상태머신의 FinalizeAfterAttack()으로 수렴
    }
}
