#include "DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameDataInstance.h"
//for git commit
bool UDeathWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    // 버튼 이벤트 바인딩
    if (RestartButton) RestartButton->OnClicked.AddDynamic(this, &UDeathWidget::OnRestartClicked);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UDeathWidget::OnExitClicked);
    return true;
}

void UDeathWidget::Setup(APlayerController* InController, bool bInIsCleared)
{
    OwningController = InController;
    bIsGameCleared = bInIsCleared;
}

void UDeathWidget::OnRestartClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;
    // 게임 데이터 초기화 및 현재 레벨 재시작
    if (UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(World))
    {
        GameData->ResetGameStatsToLevelOne();
        FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, true);
        UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
    }
}

void UDeathWidget::OnExitClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;
    // 게임 데이터 초기화 및 메인 메뉴로 이동
    if (UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(World))
    {
        GameData->ResetGameStatsToLevelOne();
        UGameplayStatics::OpenLevel(World, GameData->MainMenuLevelName);
    }
	
    // 메인 메뉴로 (L_MainMenu)
    if (OwningController)
    {
        UGameplayStatics::OpenLevel(OwningController, TEXT("L_MainMenu"));
    }
    else
    {
        UGameplayStatics::OpenLevel(World, TEXT("L_MainMenu"));
    }
}
