#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "StatsHUD.generated.h"

class AFPSCharacter; // 캐릭터 클래스 선언
//class UGunComponent;
//class GunStatus;

UCLASS()
class ROGUELIKEFPS_API UStatsHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    // 캐릭터 설정 함수 (PlayerController에서 호출)
    void SetOwningCharacter(AFPSCharacter* NewCharacter);

protected:
    // UI 컴포넌트 바인딩
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_Health; // 체력 바
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_EXP; // 경험치 바
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Level; // 레벨 텍스트
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_MinMaxBullet; // 탄약 텍스트
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_WeaponName; // 무기 이름 텍스트
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Gold; // 골드 텍스트
    //UPROPERTY(meta = (BindWidget))
    //UTextBlock* Text_CurrentAmmo; // 현재 총알 텍스트
    //UPROPERTY(meta = (BindWidget))
    //UTextBlock* Text_MaxAmmo; // 최대 총알 텍스트

    // 캐릭터 참조
    UPROPERTY()
    TObjectPtr<AFPSCharacter> OwningCharacter;

    //UPROPERTY()
    //TObjectPtr<UGunComponent> OwingGunComponet;

    //UPROPERTY()
    //TObjectPtr<GunStatus> OwingGunStatusComponet;

protected:
    // NativeTick 오버라이드
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // UI 업데이트 함수
    void UpdateHealthDisplay();
    void UpdateEXPDisplay();
    void UpdateLevel();
    //void UpdateWeaponDisplay();
    void UpdateGoldDisplay();

    // 이벤트 콜백
    UFUNCTION()
    void OnCharacterStatChanged(FName StatName);
    UFUNCTION()
    void OnCharacterDied(AController* KillerController);

    // NativeConstruct 오버라이드
    virtual void NativeConstruct() override;
};