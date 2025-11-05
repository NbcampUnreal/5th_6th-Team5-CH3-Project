// UEnemyStatsComponent.cpp
#include "EnemyStatsComponent.h"
#include "AI/Character/AIEnemyCharacter.h"

void UEnemyStatsComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (GetOwner() && GetOwner()->HasAuthority())
    {
        if (const auto* C = Cast<AAIEnemyCharacter>(GetOwner()))
        {
            if (C->Config)
            {
                InitializeFromConfig(C->Config);
            }
        }
    }
}
