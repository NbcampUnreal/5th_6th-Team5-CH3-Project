#include "DeathWidget.h"
#include "UIManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameDataInstance.h"

bool UDeathWidget::Initialize()
{
    RestartButton->OnClicked.AddDynamic(this, &UDeathWidget::OnRestartClicked);
    ExitButton->OnClicked.AddDynamic(this, &UDeathWidget::OnExitClicked);
    return true;
}

void UDeathWidget::OnRestartClicked()
{
    if (UGameDataInstance* GameData = Cast<UGameDataInstance>(GetGameInstance()))
    {
        GameData->ResetGameStatsToLevelOne();
        UGameplayStatics::OpenLevel(GetWorld(), GameData->StageLevelNames[0]);
    }
}

void UDeathWidget::OnExitClicked()
{
    if (UGameDataInstance* GameData = Cast<UGameDataInstance>(GetGameInstance()))
    {
        GameData->ResetGameStatsToLevelOne();
        if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
        {
            UIManager->ShowTitleScreen();
        }
    }
}
