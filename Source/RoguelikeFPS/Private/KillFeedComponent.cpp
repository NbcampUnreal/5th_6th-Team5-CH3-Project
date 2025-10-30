#include "KillFeedComponent.h"
#include "StatsComponent.h"
#include "GameFramework/Character.h" 

UKillFeedComponent::UKillFeedComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UKillFeedComponent::ReportDeathToKiller(AActor* KillerActor)
{
    if (!KillerActor) return;

    // 킬러 액터(플레이어)에서 UStatsComponent 찾음
    UStatsComponent* KillerStats = KillerActor->FindComponentByClass<UStatsComponent>();

    if (KillerStats)
    {
        // XP 부여 루프 실행
        KillerStats->AddXP(XPAmountOnDeath);
    }
}