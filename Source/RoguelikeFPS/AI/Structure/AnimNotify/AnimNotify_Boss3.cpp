#include "AnimNotify_Boss3.h"
#include "../Stage2BossAttackComponent.h"

void UAnimNotify_Boss3::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        //UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit start"));

        if (auto* Boss2 = Owner->FindComponentByClass<UStage2BossAttackComponent>())
        {
            UE_LOG(LogTemp, Log, TEXT("boss2 3 start"));
            Boss2->DoHit();
        }
    }

}
