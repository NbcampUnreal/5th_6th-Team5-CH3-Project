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
    // OwningController가 유효하면 그 컨텍스트로, 아니면 World로 열기
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

    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(World));
    if (GameInstance)
    {
        GameInstance->ResetGameStatsToLevelOne();
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
