#include "StatsHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameFramework/Pawn.h"
#include "GameDataInstance.h"
#include "Kismet/GameplayStatics.h"

void UStatsHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. StatsComponent 연결
    if (APawn* Pawn = Cast<APawn>(GetOwningPlayerPawn()))
    {
        StatsComp = Pawn->FindComponentByClass<UStatsComponent>();
        if (StatsComp)
        {
            StatsComp->OnHealthChanged.AddDynamic(this, &UStatsHUD::OnHealthChanged);
            OnHealthChanged(StatsComp->GetCurrentHealth(), StatsComp->GetMaxHealth());
        }
    }

    // 2. GameInstance 연결
    GameData = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    // 3. 초기 상태
    LastSelectedWeaponIndex = GameData ? GameData->SelectedWeaponIndex : 0;
    UpdateWeaponSelection();
    UpdateGold(100);
}

void UStatsHUD::NativeDestruct()
{
    if (StatsComp)
    {
        StatsComp->OnHealthChanged.RemoveDynamic(this, &UStatsHUD::OnHealthChanged);
    }
    Super::NativeDestruct();
}

void UStatsHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (GameData && GameData->SelectedWeaponIndex != LastSelectedWeaponIndex)
    {
        LastSelectedWeaponIndex = GameData->SelectedWeaponIndex;
        UpdateWeaponSelection();
    }
}

// ===== HP/EXP =====
float UStatsHUD::GetHPPercent() const
{
    if (!StatsComp) return 0.0f;
    return StatsComp->GetMaxHealth() > 0 ?
        static_cast<float>(StatsComp->GetCurrentHealth()) / StatsComp->GetMaxHealth() : 0.0f;
}

float UStatsHUD::GetEXPPercent() const
{
    if (!StatsComp) return 0.0f;
    return StatsComp->GetXPToNextLevel() > 0.0f ?
        StatsComp->GetCurrentXP() / StatsComp->GetXPToNextLevel() : 0.0f;
}

FText UStatsHUD::GetAmmoText() const
{
    return FText::Format(FText::FromString(TEXT("{0}/{1}")), CurrentAmmo, MaxAmmo);
}

FText UStatsHUD::GetGoldText() const
{
    return FText::Format(FText::FromString(TEXT("{0} Gold")), CurrentGold);
}

void UStatsHUD::OnHealthChanged(int32 NewHealth, int32 MaxHealth)
{
    if (HPBar) HPBar->SetPercent(GetHPPercent());
    if (EXPBar) EXPBar->SetPercent(GetEXPPercent());
}

// ===== 무기 선택 =====
void UStatsHUD::UpdateWeaponSelection()
{
    if (!GameData) return;

    const int32 Index = GameData->SelectedWeaponIndex;

    // 아이콘 활성화
    UpdateWeaponIcons(Index);

    // 탄약 설정 (TMap 없이 직접 하드코딩)
    switch (Index)
    {
    case 1: CurrentAmmo = 30; MaxAmmo = 300; break;  // Pistol
    case 2: CurrentAmmo = 60; MaxAmmo = 180; break;  // Rifle
    case 3: CurrentAmmo = 8;  MaxAmmo = 32;  break;  // Shotgun
    case 4: CurrentAmmo = 10; MaxAmmo = 40;  break;  // Sniper
    default: CurrentAmmo = 0; MaxAmmo = 0; break;
    }

    if (AmmoText) AmmoText->SetText(GetAmmoText());
}

void UStatsHUD::UpdateWeaponIcons(int32 ActiveIndex)
{
    if (WeaponIcon1) WeaponIcon1->SetOpacity(ActiveIndex == 1 ? 1.0f : 0.5f);
    if (WeaponIcon2) WeaponIcon2->SetOpacity(ActiveIndex == 2 ? 1.0f : 0.5f);
    if (WeaponIcon3) WeaponIcon3->SetOpacity(ActiveIndex == 3 ? 1.0f : 0.5f);
    if (WeaponIcon4) WeaponIcon4->SetOpacity(ActiveIndex == 4 ? 1.0f : 0.5f);
}

// ===== 외부 호출 =====
void UStatsHUD::UpdateAmmo(int32 NewCurrent, int32 NewMax)
{
    CurrentAmmo = FMath::Clamp(NewCurrent, 0, NewMax);
    MaxAmmo = NewMax;
    if (AmmoText) AmmoText->SetText(GetAmmoText());
}

void UStatsHUD::UpdateGold(int32 NewGold)
{
    CurrentGold = FMath::Max(0, NewGold);
    if (GoldText) GoldText->SetText(GetGoldText());
}

void UStatsHUD::ForceUpdateWeaponSelection()
{
    if (GameData)
    {
        LastSelectedWeaponIndex = -1;  // 강제 갱신 트리거
        UpdateWeaponSelection();
    }
}