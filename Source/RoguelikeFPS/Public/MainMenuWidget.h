#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UGameDataInstance;
class AFPSGameMode;

UCLASS()
class ROGUELIKEFPS_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual bool Initialize() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WeaponButton1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WeaponButton2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> WeaponButton3;

    UPROPERTY(meta = (BindWidget))  // 추가: 4번째 무기
        TObjectPtr<UButton> WeaponButton4;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget))  // 추가: Back 버튼
        TObjectPtr<UButton> BacktoTitleButton;

protected:
    UFUNCTION()
    void OnWeapon1Selected();

    UFUNCTION()
    void OnWeapon2Selected();

    UFUNCTION()
    void OnWeapon3Selected();

    UFUNCTION()  // 추가: 4번째 무기 선택
        void OnWeapon4Selected();

    UFUNCTION()
    void OnStartGameClicked();

    UFUNCTION()  // 추가: Back 버튼 클릭
        void OnBacktoTitleClicked();

private:
    UPROPERTY()
    TObjectPtr<UGameDataInstance> GameDataInstance;

    UPROPERTY()
    TObjectPtr<AFPSGameMode> GameMode;
};