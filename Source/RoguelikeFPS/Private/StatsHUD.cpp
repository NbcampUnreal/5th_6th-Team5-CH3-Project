#include "StatsHUD.h"
#include "FPSCharacter.h"
#include "FPSPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


void UStatsHUD::SetOwningCharacter(AFPSCharacter* NewCharacter)
{

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    OwningCharacter = NewCharacter;
    if (OwningCharacter)
    {
        // 캐릭터 이벤트에 콜백 함수 바인딩
        OwningCharacter->OnHUDStatChanged.AddDynamic(this, &UStatsHUD::OnCharacterStatChanged);
        OwningCharacter->OnPlayerDeath.AddDynamic(this, &UStatsHUD::OnCharacterDied);
        // 초기 데이터로 UI 업데이트
        UpdateHealthDisplay();
        UpdateEXPDisplay();
        UpdateLevel();
        //UpdateWeaponDisplay();
        UpdateGoldDisplay();
        //UpdateCurrentAmmoDisplay();
        //UpdateMaxAmmoDisplay();

    }
}

void UStatsHUD::NativeConstruct()
{
    //Super::NativeConstruct();
    //// 캐릭터가 아직 설정되지 않았다면, PlayerController를 통해 설정
    //if (!OwningCharacter)
    //{
    //    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    //    {
    //        if (AFPSCharacter* Char = Cast<AFPSCharacter>(PC->GetPawn()))
    //        {
    //            SetOwningCharacter(Char);
    //        }
    //    }
    //}
}

void UStatsHUD::OnCharacterStatChanged(FName StatName)
{
    if (StatName.IsEqual(TEXT("Health")) || StatName.IsEqual(TEXT("MaxHealth")))
    {
        UpdateHealthDisplay();
    }
    else if (StatName.IsEqual(TEXT("Experience")) || StatName.IsEqual(TEXT("MaxExperience")))
    {
        UpdateEXPDisplay();
    }
    else if (StatName.IsEqual(TEXT("Gold")))
    {
        UpdateGoldDisplay();
    }
    else if (StatName.IsEqual(TEXT("Level")))
    {
        UpdateLevel();
    }
}

void UStatsHUD::OnCharacterDied(AController* KillerController)
{
    // 사망 시 처리 (예: Game Over 화면 표시)
    // 현재는 FPSGameMode에서 GameOver 상태로 전환하므로 추가 작업 불필요
}

void UStatsHUD::UpdateHealthDisplay()
{
    if (OwningCharacter && ProgressBar_Health)
    {
        float Health = (float)OwningCharacter->GetHealth();
        float MaxHealth = (float)OwningCharacter->GetMaxHealth();
        float Percent = (MaxHealth > 0) ? Health / MaxHealth : 0.0f;
        ProgressBar_Health->SetPercent(Percent);
    }
}

void UStatsHUD::UpdateEXPDisplay()
{
    if (OwningCharacter && ProgressBar_EXP)
    {
        float CurrentExp = (float)OwningCharacter->GetExperience();
        float MaxExp = (float)OwningCharacter->GetMaxExperience();
        float Percent = (MaxExp > 0) ? CurrentExp / MaxExp : 0.0f;
        ProgressBar_EXP->SetPercent(Percent);
    }
}

void UStatsHUD::UpdateLevel()
{
    if (OwningCharacter && Text_Level)
    {
        Text_Level->SetText(FText::Format(
            NSLOCTEXT("HUD", "LevelFormat", "{0} Level"),
            FText::AsNumber(OwningCharacter->GetLevel())
        ));
    }
}

void UStatsHUD::UpdateGoldDisplay()
{
    if (OwningCharacter && Text_Gold)
    {
        Text_Gold->SetText(FText::Format(
            NSLOCTEXT("HUD", "GoldFormat", "{0} Gold"),
            FText::AsNumber(OwningCharacter->GetGoldAmount())
        ));
    }
}

//void UStatsHUD::UpdateCurrentAmmoDisplay()
//{
//    if (OwingGunComponet && CurrentBulletCount)
//    {
//        Text_CurrentAmmo->SetText(FText::Format(
//            NSLOCTEXT("HUD", "MaxAmmoFormat", "{0} CurrentAmmo"),
//            FText::AsNumber(OwingGunComponet->GetCurrentAmmount())
//        ));
//    }
//}
//void UStatsHUD::UpdateMaxAmmoDisplay()
//{
//    if (OwningCharacter && Text_MaxAmmo)
//    {
//        Text_MaxAmmo->SetText(FText::Format(
//            NSLOCTEXT("HUD", "MaxAmmoFormat", "{0} MaxAmmo"),
//            FText::AsNumber(OwingGunStatusComponet->GetMaxAmmount())
//        ));
//    }
//}

void UStatsHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    //Super::NativeTick(MyGeometry, InDeltaTime);
    // 추가적인 실시간 업데이트가 필요하면 여기에 구현
}