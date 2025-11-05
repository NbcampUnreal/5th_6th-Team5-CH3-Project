#include "KillFeedComponent.h"
#include "FPSCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

UKillFeedComponent::UKillFeedComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UKillFeedComponent::ReportDeathToKiller(AActor* KillerActor)
{
    if (!KillerActor) return;

    // KillerActor가 Pawn/Character일 가능성 고려
    AFPSCharacter* KillerCharacter = Cast<AFPSCharacter>(KillerActor);
    if (!KillerCharacter)
    {
        // 만약 KillerActor가 Pawn이고 그 Pawn의 소유자가 캐릭터라면 시도
        APawn* Pawn = Cast<APawn>(KillerActor);
        if (Pawn)
        {
            KillerCharacter = Cast<AFPSCharacter>(Pawn);
        }
    }

    if (KillerCharacter)
    {
        KillerCharacter->AddXP(XPAmountOnDeath);
    }
}
