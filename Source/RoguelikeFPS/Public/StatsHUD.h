#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "StatsHUD.generated.h"

class AFPSCharacter; // 캐릭터 클래스 참조

UCLASS()
class ROGUELIKEFPS_API UStatsHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    // --- 위젯 생성 시 캐릭터를 저장할 함수 (PlayerController가 호출) ---
    void SetOwningCharacter(AFPSCharacter* NewCharacter);

protected:
    // --- 1. 디자이너에서 만든 UI 컴포넌트 참조 ---
    // (WBP에서 Is Variable 체크 필요)
    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_Health; // ProgressBar_Health 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ProgressBar_EXP;    // ProgressBar_EXP 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_MinMaxBullet;    // MinMaxBulit 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_WeaponName;      // WeaponName 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Gold;            // Gold 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill1CD;        // Skill1 이름으로 가정

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Skill2CD;        // Skill2 이름으로 가정

    // Image 변수들은 Text/ProgressBar와 달리 Bind 대신 SetBrush 등을 직접 호출하므로 생략 가능 (필요하면 추가)

    // --- 2. 캐릭터 참조 ---
    UPROPERTY()
    TObjectPtr<AFPSCharacter> OwningCharacter;


protected:
    // --- 3. 위젯이 화면에 추가될 때 호출 ---
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // --- 4. C++에서 직접 UI 업데이트 (필요한 데이터가 변경될 때마다 호출) ---
    void UpdateHealthDisplay();
    void UpdateEXPDisplay();
    void UpdateWeaponDisplay();
    void UpdateGoldDisplay();
    void UpdateSkillCooldownDisplay();

    // --- 5. 델리게이트 콜백 (Character가 직접 호출) ---
    UFUNCTION()
    void OnCharacterStatChanged(FName StatName);

    UFUNCTION()
    void OnCharacterDied(AController* KillerController);

    // NativeConstruct 선언 추가 (빌드 오류 해결용)
    virtual void NativeConstruct() override;
};