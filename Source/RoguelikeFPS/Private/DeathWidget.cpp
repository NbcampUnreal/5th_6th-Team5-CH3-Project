#include "DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameDataInstance.h"

bool UDeathWidget::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UDeathWidget::OnRestartClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UDeathWidget::OnExitClicked);
    }

    return true;
}

void UDeathWidget::OnRestartClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(World));
    if (GameInstance)
    {
        GameInstance->ResetGameStatsToLevelOne();
    }

    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(World, true);

    // 현재 레벨로 재시작
    if (OwningController)
    {
        UGameplayStatics::OpenLevel(OwningController, FName(*CurrentLevelName));
    }
    else
    {
        UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
    }
}

void UDeathWidget::OnExitClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);
    UGameDataInstance* GameDataInstance = Cast<UGameDataInstance>(GameInstance);

    if (GameDataInstance)
    {
        GameDataInstance->ResetGameStatsToLevelOne();

        // 메인 메뉴 이름 사용
        FName MenuLevelName = GameDataInstance->MainMenuLevelName;

        if (OwningController)
        {
            UGameplayStatics::OpenLevel(OwningController, MenuLevelName);
        }
        else
        {
            UGameplayStatics::OpenLevel(World, MenuLevelName);
        }
    }
}