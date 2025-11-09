#include "ClearWidget.h"
#include "GameDataInstance.h"
#include "UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

bool UClearWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    if (RestartButton)
        RestartButton->OnClicked.AddDynamic(this, &UClearWidget::OnRestartClicked);
    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UClearWidget::OnExitClicked);
    GameDataInstance = Cast<UGameDataInstance>(GetGameInstance());
    UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
    return true;
}

void UClearWidget::OnRestartClicked()
{
    if (GameDataInstance)
    {
        GameDataInstance->bGameStarted = true;
        GameDataInstance->CurrentStageIndex = 0;
        GameDataInstance->TeleportCount = 0;
        if (UIManager)
        {
            UIManager->HideAllWidgetsExceptLayer(EUILayer::Background);
            UIManager->SetInputModeGame();
        }
        UGameplayStatics::OpenLevel(GetWorld(), GameDataInstance->StageLevelNames[0]);
    }
}

void UClearWidget::OnExitClicked()
{
    if (UIManager)
    {
        UIManager->ShowTitleScreen();
    }
    if (GameDataInstance)
    {
        GameDataInstance->bGameStarted = false;
        GameDataInstance->TeleportCount = 0;
    }
}