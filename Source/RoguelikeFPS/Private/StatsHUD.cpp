// StatsHUD.cpp
#include "StatsHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"

void UStatsHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (APawn* Pawn = Cast<APawn>(GetOwningPlayerPawn()))
    {
        StatsComp = Pawn->FindComponentByClass<UStatsComponent>();
        if (StatsComp)
        {
            StatsComp->OnHealthChanged.AddDynamic(this, &UStatsHUD::OnHealthChanged);

            // 수정: GetMaxHealth() 사용
            OnHealthChanged(StatsComp->GetCurrentHealth(), StatsComp->GetMaxHealth());

            if (EXPBar) EXPBar->SetPercent(GetEXPPercent());
            if (AmmoText) AmmoText->SetText(GetAmmoText());
        }
    }
}

void UStatsHUD::NativeDestruct()
{
    if (StatsComp)
    {
        StatsComp->OnHealthChanged.RemoveDynamic(this, &UStatsHUD::OnHealthChanged);
    }
    Super::NativeDestruct();
}

float UStatsHUD::GetHPPercent() const
{
    if (!StatsComp) return 0.0f;
    const int32 Current = StatsComp->GetCurrentHealth();
    const int32 Max = StatsComp->GetMaxHealth(); // Getter 사용
    return Max > 0 ? static_cast<float>(Current) / Max : 0.0f;
}

float UStatsHUD::GetEXPPercent() const
{
    if (!StatsComp) return 0.0f;
    const float Current = StatsComp->GetCurrentXP();
    const float Required = StatsComp->GetXPToNextLevel();
    return Required > 0.0f ? Current / Required : 0.0f;
}

FText UStatsHUD::GetAmmoText() const
{
    return FText::AsNumber(CurrentAmmo);
}

void UStatsHUD::OnHealthChanged(int32 NewHealth, int32 MaxHealth)
{
    if (HPBar)
    {
        HPBar->SetPercent(GetHPPercent());
    }
    if (EXPBar)
    {
        EXPBar->SetPercent(GetEXPPercent());
    }
}

void UStatsHUD::SetAmmo(int32 NewAmmo)
{
    CurrentAmmo = FMath::Max(0, NewAmmo);
    if (AmmoText)
    {
        AmmoText->SetText(GetAmmoText());
    }
}