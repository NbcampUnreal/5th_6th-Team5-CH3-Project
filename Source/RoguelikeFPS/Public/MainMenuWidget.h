// Fill out your copyright notice in the Description page of Project Settings.

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
    // 무기 옵션 버튼 (UMG에서 변수 이름 일치 필수: Button_Weapon1, Button_Weapon2 등)
    UPROPERTY(meta = (BindWidget)) UButton* Button_Weapon1;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Weapon2;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Weapon3;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Weapon4;

    // 시작 및 종료 버튼
    UPROPERTY(meta = (BindWidget)) UButton* Button_Start;
    UPROPERTY(meta = (BindWidget)) UButton* Button_Exit;

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

    // 시작 및 종료 버튼 클릭 핸들러
    UFUNCTION()
    void OnStartGameClicked();
    UFUNCTION()
    void OnExitGameClicked();

private:
    // 옵션 선택 공통 함수 (GameDataInstance의 SelectedWeaponIndex를 업데이트)
    void HandleWeaponSelection(int32 WeaponIndex);
};
