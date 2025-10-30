// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Structure/AnimNotify/AnimNotify_DeadExit.h"

void UAnimNotify_DeadExit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp) return;
    if (AActor* Owner = MeshComp->GetOwner())
    {
        UE_LOG(LogTemp, Log, TEXT("AnimNotify_DeadExit start"));
        //Owner->SetLifeSpan(0.5f);
        Owner->Destroy();
    }
}
