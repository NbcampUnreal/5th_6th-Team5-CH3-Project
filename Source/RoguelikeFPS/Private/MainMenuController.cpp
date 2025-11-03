#include "MainMenuController.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AMainMenuController::BeginPlay()
{
    Super::BeginPlay();

    if (WITH_EDITOR && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MainMenuController BeginPlay Called! Starting with TitleWidget."));
    }

    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);

            FInputModeUIOnly InputModeData;

            InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());

            SetInputMode(InputModeData);
            bShowMouseCursor = true;
            return;
        }
    }

    // fallback
    FInputModeGameAndUI InputModeData;
    SetInputMode(InputModeData);
    bShowMouseCursor = true;
}

void AMainMenuController::ShowWeaponSelectMenu()
{
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

            FInputModeUIOnly InputModeData;

            InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());

            SetInputMode(InputModeData);
            bShowMouseCursor = true;
        }
    }
}

void AMainMenuController::ShowTitleScreen()
{
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

            FInputModeUIOnly InputModeData;

            InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());

            SetInputMode(InputModeData);
            bShowMouseCursor = true;
        }
    }
}
