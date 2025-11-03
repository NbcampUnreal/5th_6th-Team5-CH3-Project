#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class ROGUELIKEFPS_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

public:
    //뒤로가기 버튼 클릭시 호출할 델리게이트
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonClicked);
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBackButtonClicked OnBackButtonClicked;
    // 무기 옵션 버튼 (UMG에서 변수 이름 일치 필수: Button_Weapon1, Button_Weapon2 등)
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Weapon1;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Weapon2;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Weapon3;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Weapon4;

    // 시작 및 종료 버튼
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Start;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> Button_Exit;

protected:
    // 무기 버튼 클릭 래퍼 함수들
    UFUNCTION()
    void OnButtonWeapon1Clicked();
    UFUNCTION()
    void OnButtonWeapon2Clicked();
    UFUNCTION()
    void OnButtonWeapon3Clicked();
    UFUNCTION()
    void OnButtonWeapon4Clicked();

    // 시작 버튼 클릭 핸들러
    UFUNCTION()
    void OnStartGameClicked();

    // 옵션 선택 공통 함수 (GameDataInstance의 SelectedWeaponIndex를 업데이트)
    void HandleWeaponSelection(int32 WeaponIndex);

private:
    // [유지] 뒤로가기 버튼 클릭 핸들러로 사용
    UFUNCTION()
    void OnExitButtonClicked(); // Button_Exit에 이 함수를 바인딩하여 델리게이트를 발송합니다.
};