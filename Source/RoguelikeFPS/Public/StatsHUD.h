#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsComponent.h"
#include "GameDataInstance.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "StatsHUD.generated.h"

UCLASS()
class ROGUELIKEFPS_API UStatsHUD : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // ===== 바인딩 함수 (HP/EXP) =====
    UFUNCTION()
    float GetHPPercent() const;

    UFUNCTION()
    float GetEXPPercent() const;

    // ===== 바인딩 함수 (Ammo/Gold) =====
    UFUNCTION()
    FText GetAmmoText() const;

    UFUNCTION()
    FText GetGoldText() const;

    // ===== 델리게이트 =====
    UFUNCTION()
    void OnHealthChanged(int32 NewHealth, int32 MaxHealth);

public:
    UFUNCTION(BlueprintCallable)
    void ForceUpdateWeaponSelection();

    // ===== 무기/골드 업데이트 =====
    UFUNCTION(BlueprintCallable)
    void UpdateAmmo(int32 NewCurrent, int32 NewMax);

    UFUNCTION(BlueprintCallable)
    void UpdateGold(int32 NewGold);

private:
    // ===== 컴포넌트 참조 =====
    UPROPERTY()
    TObjectPtr<UStatsComponent> StatsComp;

    UPROPERTY()
    TObjectPtr<UGameDataInstance> GameData;

    // ===== 상태 변수 =====
    UPROPERTY()
    int32 CurrentAmmo = 0;

    UPROPERTY()
    int32 MaxAmmo = 0;

    UPROPERTY()
    int32 CurrentGold = 0;

    UPROPERTY()
    int32 LastSelectedWeaponIndex = -1;

    // ===== 무기 탄약 맵 =====
    //UPROPERTY()
    //class TMap<int32, FWeaponAmmoData> WeaponAmmoMap;

    // ===== UI 위젯 (BindWidget) =====
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EXPBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;  // 단일 선언!

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoldText;

    // ===== 무기 아이콘 (신규) =====
    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon1;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon2;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon3;

    UPROPERTY(meta = (BindWidget))
    class UImage* WeaponIcon4;

    // ===== 내부 함수 =====
    void UpdateWeaponSelection();
    void UpdateWeaponIcons(int32 ActiveIndex);
};