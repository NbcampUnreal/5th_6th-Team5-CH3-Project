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

    // 버튼 이벤트 바인딩
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UDeathWidget::OnRestartClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UDeathWidget::OnExitClicked);
    }

    // OwningController는 AFPSCharacter의 OnDeath에서 설정된다고 가정합니다.

    return true;
}

void UDeathWidget::OnRestartClicked()
{
    if (!OwningController) return;

    // 1. GameInstance를 가져와 스탯 초기화 (Level 1로 초기화)
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        GameInstance->ResetGameStatsToLevelOne();
    }

    // 2. 현재 레벨을 다시 로드 (다시하기)
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(OwningController, FName(*CurrentLevelName));
}

void UDeathWidget::OnExitClicked()
{
    if (!OwningController) return;

    // 1. GameInstance를 가져와 스탯 초기화 (메인 메뉴로 나가기 전에도 초기화)
    UGameDataInstance* GameInstance = Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        GameInstance->ResetGameStatsToLevelOne();
    }

    // 2. 메인 메뉴 레벨로 이동 (끝내기)
    UGameplayStatics::OpenLevel(OwningController, TEXT("L_MainMenu"));
}