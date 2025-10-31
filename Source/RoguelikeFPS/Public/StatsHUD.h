#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsComponent.h"
#include "StatsHUD.generated.h"

UCLASS()
class ROGUELIKEFPS_API UStatsHUD : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // 바인딩 함수
    UFUNCTION()
    float GetHPPercent() const;

    UFUNCTION()
    float GetEXPPercent() const;

    UFUNCTION()
    FText GetAmmoText() const;

    // 델리게이트
    UFUNCTION()
    void OnHealthChanged(int32 NewHealth, int32 MaxHealth);

    // 탄약 갱신 (외부에서 호출)
    UFUNCTION()
    void SetAmmo(int32 NewAmmo);

private:
    UPROPERTY()
    TObjectPtr<UStatsComponent> StatsComp;

    UPROPERTY()
    int32 CurrentAmmo = 0;

    // BindWidget
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EXPBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AmmoText;
};