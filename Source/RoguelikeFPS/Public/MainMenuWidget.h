#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;
    virtual void NativeOnInitialized() override;

public:
    // ===== 델리게이트 =====
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuReady);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonClicked);

    UPROPERTY(BlueprintAssignable, Category = "UI")
    FOnMainMenuReady OnMainMenuReady;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBackButtonClicked OnBackButtonClicked;

    // ===== 버튼 바인딩 =====
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Weapon1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Weapon2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Weapon3;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Weapon4;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Start;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_Exit;

protected:
    // ===== 무기 선택 핸들러 =====
    UFUNCTION() void OnButtonWeapon1Clicked();
    UFUNCTION() void OnButtonWeapon2Clicked();
    UFUNCTION() void OnButtonWeapon3Clicked();
    UFUNCTION() void OnButtonWeapon4Clicked();

    // ===== 시작 / 종료 버튼 =====
    UFUNCTION() void OnStartGameClicked();
    UFUNCTION() void OnBackToTitleClicked();

    // ===== 공통 선택 로직 =====
    void HandleWeaponSelection(int32 WeaponIndex);

    // 현재 선택된 무기 인덱스 캐시
    int32 CurrentSelectedWeaponIndex = 0;

};