#include "MainMenuWidget.h"
#include "GameDataInstance.h"
#include "FPSGameMode.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (WeaponButton1)
        WeaponButton1->OnClicked.AddDynamic(this, &UMainMenuWidget::OnWeapon1Selected);
    if (WeaponButton2)
        WeaponButton2->OnClicked.AddDynamic(this, &UMainMenuWidget::OnWeapon2Selected);
    if (WeaponButton3)
        WeaponButton3->OnClicked.AddDynamic(this, &UMainMenuWidget::OnWeapon3Selected);
    if (WeaponButton4)
        WeaponButton4->OnClicked.AddDynamic(this, &UMainMenuWidget::OnWeapon4Selected);
    if (StartGameButton)
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameClicked);
    if (BacktoTitleButton)
        BacktoTitleButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBacktoTitleClicked);

    GameDataInstance = Cast<UGameDataInstance>(GetGameInstance());
    GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

    if (StartGameButton)
    {
        StartGameButton->SetIsEnabled(false);
    }
    if (GameDataInstance)
    {
        GameDataInstance->IsWeaponSelected = false;
        GameDataInstance->SelectedWeaponIndex = -1;
    }

    return true;
}

void UMainMenuWidget::OnWeapon1Selected()
{
    if (GameDataInstance)
    {
        GameDataInstance->SelectedWeaponIndex = 0;
        GameDataInstance->IsWeaponSelected = true;
        if (StartGameButton)
            StartGameButton->SetIsEnabled(true);
        UE_LOG(LogTemp, Log, TEXT("w1"));
    }
}

void UMainMenuWidget::OnWeapon2Selected()
{
    if (GameDataInstance)
    {
        GameDataInstance->SelectedWeaponIndex = 1;
        GameDataInstance->IsWeaponSelected = true;
        if (StartGameButton)
            StartGameButton->SetIsEnabled(true);
        UE_LOG(LogTemp, Log, TEXT("w2"));
    }
}

void UMainMenuWidget::OnWeapon3Selected()
{
    if (GameDataInstance)
    {
        GameDataInstance->SelectedWeaponIndex = 2;
        GameDataInstance->IsWeaponSelected = true;
        if (StartGameButton)
            StartGameButton->SetIsEnabled(true);
        UE_LOG(LogTemp, Log, TEXT("w3"));
    }
}

void UMainMenuWidget::OnWeapon4Selected()
{
    if (GameDataInstance)
    {
        GameDataInstance->SelectedWeaponIndex = 3;
        GameDataInstance->IsWeaponSelected = true;
        if (StartGameButton)
            StartGameButton->SetIsEnabled(true);
        UE_LOG(LogTemp, Log, TEXT("w4"));
    }
}

void UMainMenuWidget::OnStartGameClicked()
{
    if (GameDataInstance && GameDataInstance->IsWeaponSelected)
    {
        GameDataInstance->bGameStarted = true;
        GameDataInstance->CurrentStageIndex = 0;
        if (GameMode)
        {
            GameMode->SetGameState(EGameState::Playing);
        }
        if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
        {
            UIManager->HideAllWidgetsExceptLayer(EUILayer::Background);
            UIManager->SetInputModeGame();
        }
        UGameplayStatics::OpenLevel(GetWorld(), GameDataInstance->StageLevelNames[0]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("무기가 선택되지 않았습니다!"));
    }
}

void UMainMenuWidget::OnBacktoTitleClicked()
{
    if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
    {
        UIManager->ShowTitleScreen();
    }
    if (GameDataInstance)
    {
        GameDataInstance->IsWeaponSelected = false;
        GameDataInstance->SelectedWeaponIndex = -1;
    }
}