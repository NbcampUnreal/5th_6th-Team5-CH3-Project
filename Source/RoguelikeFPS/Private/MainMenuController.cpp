#include "MainMenuController.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AMainMenuController::BeginPlay()
{
    Super::BeginPlay();
    // 타이틀 화면 표시
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);
            SetUIMode(TitleWidgetInstance.Get());
        }
    }
}

void AMainMenuController::SetUIMode(UUserWidget* Widget)
{
    // UI 입력 모드 설정
    if (!Widget) return;
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(Widget->TakeWidget());
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}

void AMainMenuController::ShowWeaponSelectMenu()
{
    // 메인 메뉴로 전환
    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }
    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();
            MainMenuWidgetInstance->OnBackButtonClicked.AddDynamic(this, &AMainMenuController::ShowTitleScreen);
            SetUIMode(MainMenuWidgetInstance.Get());
        }
    }
}

void AMainMenuController::ShowTitleScreen()
{
    // 타이틀 화면으로 복귀
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);
            SetUIMode(TitleWidgetInstance.Get());
        }
    }
}