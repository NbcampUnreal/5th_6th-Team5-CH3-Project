//#include "MainMenuController.h"
//#include "TitleWidget.h"
//#include "MainMenuWidget.h"
//#include "Blueprint/UserWidget.h"
//#include "Engine/Engine.h"
//#include "GameFramework/PlayerController.h"
//
//void AMainMenuController::BeginPlay()
//{
//    Super::BeginPlay();
//
//    if (TitleWidgetClass)
//    {
//        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
//        if (TitleWidgetInstance)
//        {
//            TitleWidgetInstance->AddToViewport();
//            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);
//
//            FInputModeUIOnly InputModeData;
//            InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
//            SetInputMode(InputModeData);
//            bShowMouseCursor = true;
//        }
//    }
//}
//void AMainMenuController::ShowWeaponSelectMenu()
//{
//    if (TitleWidgetInstance)
//    {
//        TitleWidgetInstance->RemoveFromParent();
//        TitleWidgetInstance = nullptr;
//    }
//
//    if (MainMenuWidgetClass)
//    {
//        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
//        if (MainMenuWidgetInstance)
//        {
//            MainMenuWidgetInstance->AddToViewport();
//            MainMenuWidgetInstance->OnBacktoTitleClicked.AddDynamic(this, &AMainMenuController::ShowTitleScreen);
//
//            FInputModeUIOnly InputModeData;
//            InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
//            SetInputMode(InputModeData);
//            bShowMouseCursor = true;
//        }
//    }
//}
//
//void AMainMenuController::ShowTitleScreen()
//{
//    if (MainMenuWidgetInstance)
//    {
//        MainMenuWidgetInstance->RemoveFromParent();
//        MainMenuWidgetInstance = nullptr;
//    }
//
//    if (TitleWidgetClass)
//    {
//        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);
//        if (TitleWidgetInstance)
//        {
//            TitleWidgetInstance->AddToViewport();
//            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);
//
//            FInputModeUIOnly InputModeData;
//            InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
//            SetInputMode(InputModeData);
//            bShowMouseCursor = true;
//        }
//    }
//}