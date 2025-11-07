#include "StatsHUD.h"
#include "FPSCharacter.h" // 캐릭터 클래스 포함
#include "FPSPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void UStatsHUD::SetOwningCharacter(AFPSCharacter* NewCharacter)
{
    OwningCharacter = NewCharacter;
    if (OwningCharacter)
    {
        // 캐릭터 델리게이트에 이 위젯의 콜백 함수 바인딩 (핵심)
        OwningCharacter->OnHUDStatChanged.AddDynamic(this, &UStatsHUD::OnCharacterStatChanged);
        OwningCharacter->OnPlayerDeath.AddDynamic(this, &UStatsHUD::OnCharacterDied);

        // 초기 데이터로 한 번 모두 업데이트 (선택 사항)
        UpdateHealthDisplay();
        UpdateEXPDisplay();
        UpdateWeaponDisplay();
        UpdateGoldDisplay();
        UpdateSkillCooldownDisplay();
    }
}

void UStatsHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // PlayerController::BeginPlay에서 SetOwningCharacter를 호출하므로,
    // 이 함수에서의 캐릭터 획득 시도는 제거/주석 처리하는 것이 좋습니다.
    /*
    if (!OwningCharacter)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            if (AFPSCharacter* Char = Cast<AFPSCharacter>(PC->GetCharacter()))
            {
                SetOwningCharacter(Char);
            }
        }
    }
    */
}

void UStatsHUD::OnCharacterStatChanged(FName StatName)
{
    // 캐릭터에서 받은 신호에 따라 필요한 부분만 업데이트
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
    else if (StatName.IsEqual(TEXT("Ammo")))
    {
        UpdateWeaponDisplay();
    }
    //else if (StatName.IsEqual(TEXT("Skill1CD")) || StatName.IsEqual(TEXT("Skill2CD")))
    //{
        //UpdateSkillCooldownDisplay();
    //}
    // Death 등 다른 이벤트 처리
}

void UStatsHUD::OnCharacterDied(AController* KillerController)
{
    // 사망 시 처리 (예: Game Over 화면 표시)
    // 이 시점에서 HUD를 제거하거나, Game Over 위젯을 띄울 수 있습니다.
}

// --- UI 업데이트 함수 구현 (Getter 함수 사용하도록 수정) ---

void UStatsHUD::UpdateHealthDisplay()
{
    if (OwningCharacter && ProgressBar_Health)
    {
        // **Getter 사용으로 수정**
        float Health = (float)OwningCharacter->GetHealth();
        float MaxHealth = (float)OwningCharacter->GetMaxHealth();

        float Percent = (Health > 0) ? Health / MaxHealth : 0.0f;
        ProgressBar_Health->SetPercent(Percent);
    }
}

void UStatsHUD::UpdateEXPDisplay()
{
    if (OwningCharacter && ProgressBar_EXP)
    {
        // **Getter 사용으로 수정**
        float CurrentExp = (float)OwningCharacter->GetExperience();
        float MaxExp = (float)OwningCharacter->GetMaxExperience();

        float Percent = (MaxExp > 0) ? CurrentExp / MaxExp : 0.0f;
        ProgressBar_EXP->SetPercent(Percent);
    }
}

void UStatsHUD::UpdateWeaponDisplay()
{
    if (OwningCharacter && Text_WeaponName && Text_MinMaxBullet)
    {
        // **Getter 사용으로 수정**
        Text_WeaponName->SetText(FText::FromName(OwningCharacter->GetCurrentWeaponName()));

        Text_MinMaxBullet->SetText(FText::Format(
            NSLOCTEXT("HUD", "AmmoFormat", "{0} / {1}"),
            FText::AsNumber(OwningCharacter->GetCurrentAmmo()),
            FText::AsNumber(OwningCharacter->GetMaxAmmo())
        ));
    }
}

void UStatsHUD::UpdateGoldDisplay()
{
    if (OwningCharacter && Text_Gold)
    {
        // **Getter 사용으로 수정**
        Text_Gold->SetText(FText::Format(
            NSLOCTEXT("HUD", "GoldFormat", "{0} Gold"),
            FText::AsNumber(OwningCharacter->GetGoldAmount())
        ));
    }
}

//void UStatsHUD::UpdateSkillCooldownDisplay()
//{
//    if (OwningCharacter && Skill1CD)
//    {
//        // **Getter 사용으로 수정**
//        float Remaining = OwningCharacter->GetSkill1CooldownRemaining();
//        Skill1CD->SetText(FText::AsNumber(FMath::CeilToInt(Remaining)));
//    }
//    if (OwningCharacter && Skill2CD)
//    {
//        // **Getter 사용으로 수정**
//        float Remaining = OwningCharacter->GetSkill2CooldownRemaining();
//        Skill2CD->SetText(FText::AsNumber(FMath::CeilToInt(Remaining)));
//    }
//}

void UStatsHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Tick 로직은 여기에 작성하거나, 필요 없으면 Super::만 호출
    if (OwningCharacter)
    {
        // 필요하다면 쿨다운 등을 여기서 업데이트
    }
}