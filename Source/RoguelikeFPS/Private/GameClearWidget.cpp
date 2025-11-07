#include "GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h" 
#include "Blueprint/UserWidget.h"
#include "GameDataInstance.h" // GameDataInstance 포함

bool UGameClearWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnExitButtonClicked);

    return true;
}

void UGameClearWidget::SetupGameModeLink(AFPSGameMode* OwningGameMode)
{
    OwningGameModePtr = OwningGameMode;
}


void UGameClearWidget::OnExitButtonClicked()
{
    UWorld* World = GetWorld();
    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);

    if (GameInstance)
    {
        if (UGameDataInstance* GameDataInstance = Cast<UGameDataInstance>(GameInstance))
        {
            // 클리어 후 다음 플레이를 위해 게임 데이터 초기화 (권장)
            GameDataInstance->ResetGameStatsToLevelOne();
        }
    }

    // 메인 메뉴 레벨로 이동
    FName MenuLevelName = TEXT("L_MainMenu"); // 기본값
    if (GameInstance && Cast<UGameDataInstance>(GameInstance))
    {
        MenuLevelName = Cast<UGameDataInstance>(GameInstance)->MainMenuLevelName;
    }

    // UI 제거 및 레벨 전환
    RemoveFromParent();
    UGameplayStatics::OpenLevel(World, MenuLevelName);
}