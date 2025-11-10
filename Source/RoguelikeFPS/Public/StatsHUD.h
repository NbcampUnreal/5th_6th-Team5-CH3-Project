#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "StatsHUD.generated.h"

class AFPSCharacter;
class UGunComponent;

// StatsHUD.h
void HideHUD();
void ShowHUD();

UCLASS()
class ROGUELIKEFPS_API UStatsHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetOwningCharacter(AFPSCharacter* NewCharacter);
    // 외부에서 호출 → HUD 숨기기/표시
    void HideHUD();
    void ShowHUD();

protected:
    // 현재 HUD 상태
    bool bIsHUDVisible = true;
    // UI 위젯 (WBP_StatsHUD 에서 이름 정확히 일치 + BindWidget 체크!)
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_Health;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_Shield;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_EXP;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Level;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_WeaponName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Ammo;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Gold;

    UPROPERTY(meta = (BindWidget))
    UImage* DashIndicator;  // 대시 중일 때 색상 변경 (옵션)

    // ---------- 4개의 스킬 ----------
    UPROPERTY(meta = (BindWidget))
    UImage* Skill1_Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill1_CD;

    UPROPERTY(meta = (BindWidget))
    UImage* Skill2_Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill2_CD;

    UPROPERTY(meta = (BindWidget))
    UImage* Skill3_Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill3_CD;

    UPROPERTY(meta = (BindWidget))
    UImage* Skill4_Icon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill4_CD;

    // 참조
    UPROPERTY()
    TObjectPtr<AFPSCharacter> OwningCharacter;

    UPROPERTY()
    TObjectPtr<UGunComponent> GunComponent;

    // 라이프사이클
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // UI 업데이트 함수
    void UpdateHealthDisplay();
    void UpdateShieldDisplay();
    void UpdateEXPDisplay();
    void UpdateLevelDisplay();
    void UpdateWeaponDisplay();
    void UpdateGoldDisplay();
    void UpdateDashIndicator();
    void UpdateSkillCooldownDisplay();

    // 델리게이트 콜백
    UFUNCTION()
    void OnCharacterStatChanged(FName StatName);

    UFUNCTION()
    void OnCharacterDied(AController* KillerController);

    
};