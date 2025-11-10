// StatsHUD.cpp
#include "StatsHUD.h"
#include "FPSCharacter.h"
#include "Weapon/GunComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameDataInstance.h"
void UStatsHUD::SetOwningCharacter(AFPSCharacter * NewCharacter)
{
    OwningCharacter = NewCharacter;
    if (OwningCharacter)
    {
        // GunComponent 찾기
        GunComponent = Cast<UGunComponent>(OwningCharacter->GetComponentByClass(UGunComponent::StaticClass()));
        // 델리게이트 바인딩
        OwningCharacter->OnHUDStatChanged.AddDynamic(this, &UStatsHUD::OnCharacterStatChanged);
        OwningCharacter->OnPlayerDeath.AddDynamic(this, &UStatsHUD::OnCharacterDied);
        // GameDataInstance 연동
        GameDataInstance = UGameDataInstance::GetGameDataInstance(GetWorld());
        if (GameDataInstance)
        {
           // GameDataInstance->OnWeaponSelected.AddDynamic(this, &UStatsHUD::OnWeaponSelectedChanged);
        }
        // 초기화
        UpdateAllDisplays();
        UpdateWeaponIcons();  // 무기 아이콘 초기화
    }
}
void UStatsHUD::HideHUD()
{
    SetVisibility(ESlateVisibility::Hidden);
}
void UStatsHUD::ShowHUD()
{
    SetVisibility(ESlateVisibility::Visible);
}
void UStatsHUD::NativeConstruct()
{
    Super::NativeConstruct();
    if (!OwningCharacter)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            if (AFPSCharacter* Char = Cast<AFPSCharacter>(PC->GetPawn()))
            {
                SetOwningCharacter(Char);
            }
        }
    }
}
void UStatsHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (OwningCharacter)
    {
        UpdateDashIndicator();           // 실시간 대시 상태
        UpdateSkillCooldownDisplay();    // 실시간 쿨타임
        UpdateWeaponDisplay();           // 실시간 탄약
    }
}
// UI 업데이트 함수들 (Getter 사용)
void UStatsHUD::UpdateHealthDisplay()
{
    if (OwningCharacter && ProgressBar_Health)
    {
        int32 Health = OwningCharacter->GetHealth();
        int32 MaxHealth = OwningCharacter->GetMaxHealth();
        float Percent = (MaxHealth > 0) ? (float)Health / MaxHealth : 0.f;
        ProgressBar_Health->SetPercent(Percent);
    }
}
void UStatsHUD::UpdateShieldDisplay()
{
    if (OwningCharacter && ProgressBar_Shield)
    {
        int32 Shield = OwningCharacter->GetShield();
        int32 MaxShield = 100; // 필요 시 캐릭터에 Getter 추가
        float Percent = (MaxShield > 0) ? (float)Shield / MaxShield : 0.f;
        ProgressBar_Shield->SetPercent(Percent);
    }
}
void UStatsHUD::UpdateEXPDisplay()
{
    if (OwningCharacter && ProgressBar_EXP)
    {
        int32 Exp = OwningCharacter->GetExperience();
        int32 MaxExp = OwningCharacter->GetMaxExperience();
        float Percent = (MaxExp > 0) ? (float)Exp / MaxExp : 0.f;
        ProgressBar_EXP->SetPercent(Percent);
    }
}
void UStatsHUD::UpdateLevelDisplay()
{
    if (OwningCharacter && Text_Level)
    {
        Text_Level->SetText(FText::Format(
            NSLOCTEXT("HUD", "Level", "Lv. {0}"),
            FText::AsNumber(OwningCharacter->GetLevel())
        ));
    }
}
void UStatsHUD::UpdateWeaponDisplay()
{
    if (!OwningCharacter) return;
    FName WeaponName = OwningCharacter->GetCurrentWeaponName();
    if (Text_WeaponName)
    {
        Text_WeaponName->SetText(FText::FromName(WeaponName));
    }
    if (Text_Ammo && GunComponent)
    {
        int32 Current = GunComponent->GetBulletCount();
        int32 Max = GunComponent->GetMaxBulletCount();
        Text_Ammo->SetText(FText::Format(
            NSLOCTEXT("HUD", "Ammo", "{0}/{1}"),
            FText::AsNumber(Current),
            FText::AsNumber(Max)
        ));
    }
}
void UStatsHUD::UpdateGoldDisplay()
{
    if (OwningCharacter && Text_Gold)
    {
        /*Text_Gold->SetText(FText::Format(
            NSLOCTEXT("HUD", "Gold", "{0} G"),
            FText::AsNumber(OwningCharacter->GetGoldAmount())
        ));*/
    }
}
void UStatsHUD::UpdateDashIndicator()
{
    if (OwningCharacter && DashIndicator)
    {
        bool bDashing = OwningCharacter->GetIsDash();
        // 예: 대시 중일 때 색상 변경 (UMG Material 필요)
        // DashIndicator->SetBrushTintColor(bDashing ? FSlateColor(FLinearColor::Red) : FSlateColor(FLinearColor::White));
    }
}
void UStatsHUD::UpdateSkillCooldownDisplay()
{
    if (!OwningCharacter) return;
    // Skill1
    if (Skill1_CD)
    {
        float CD = OwningCharacter->GetSkill1CooldownRemaining();
        Skill1_CD->SetText(CD > 0.1f ? FText::AsNumber(FMath::CeilToFloat(CD)) : FText::GetEmpty());
        Skill1_CD->SetVisibility(CD > 0.1f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    // Skill2 ~ Skill4 (동일 로직)
    if (Skill2_CD)
    {
        float CD = OwningCharacter->GetSkill2CooldownRemaining();
        Skill2_CD->SetText(CD > 0.1f ? FText::AsNumber(FMath::CeilToFloat(CD)) : FText::GetEmpty());
        Skill2_CD->SetVisibility(CD > 0.1f ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    // Skill3, Skill4 필요 시 추가
}
// ================================
// 무기 아이콘 업데이트 (GameData 연동)
// ================================
void UStatsHUD::UpdateWeaponIcons()
{
    if (!GameDataInstance) return;
    int32 SelectedIndex = GameDataInstance->GetSelectedOption();
    SetIconVisibility(WeaponIcon1, SelectedIndex == 1);
    SetIconVisibility(WeaponIcon2, SelectedIndex == 2);
    SetIconVisibility(WeaponIcon3, SelectedIndex == 3);
    SetIconVisibility(WeaponIcon4, SelectedIndex == 4);
}
void UStatsHUD::SetIconVisibility(UImage* Icon, bool bIsSelected)
{
    if (!Icon) return;
    if (bIsSelected)
    {
        // 선택됨: 불투명 + 흰색
        Icon->SetColorAndOpacity(FLinearColor::White);
        Icon->SetOpacity(1.0f);
    }
    else
    {
        // 선택 안됨: 반투명 + 회색
        Icon->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
    }
}
// ================================
// 델리게이트 콜백
// ================================
void UStatsHUD::OnCharacterStatChanged(FName StatName)
{
    if (StatName == TEXT("Health") || StatName == TEXT("MaxHealth"))
        UpdateHealthDisplay();
    else if (StatName == TEXT("Shield"))
        UpdateShieldDisplay();
    else if (StatName == TEXT("Experience") || StatName == TEXT("MaxExperience"))
        UpdateEXPDisplay();
    else if (StatName == TEXT("Level"))
        UpdateLevelDisplay();
    else if (StatName == TEXT("Weapon") || StatName == TEXT("Ammo"))
        UpdateWeaponDisplay();
    else if (StatName == TEXT("Gold"))
        UpdateGoldDisplay();
    else if (StatName == TEXT("Dash"))
        UpdateDashIndicator();
}
void UStatsHUD::OnCharacterDied(AController* KillerController)
{
}
UFUNCTION()
void UStatsHUD::OnWeaponSelectedChanged(int32 NewIndex)
{
    UE_LOG(LogTemp, Log, TEXT("StatsHUD: Weapon Selected = %d"), NewIndex);
    UpdateWeaponIcons();  // 실시간 반영
}
void UStatsHUD::UpdateAllDisplays()
{
    UpdateHealthDisplay();
    UpdateShieldDisplay();
    UpdateEXPDisplay();
    UpdateLevelDisplay();
    UpdateWeaponDisplay();
    UpdateGoldDisplay();
    UpdateDashIndicator();
    UpdateSkillCooldownDisplay();
}