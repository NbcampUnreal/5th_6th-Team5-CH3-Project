#include "GameClearWidget.h"
#include "GameDataInstance.h"
#include <StatsHUD.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


void UGameClearWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // GameDataInstance 가져오기
    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(this);
    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("GameDataInstance is null in GameClearWidget!"));
        return;
    }

    // GameOverButton 바인딩
    if (GameOverButton)
    {
        GameOverButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnGameOverClicked);
    }
}

void UGameClearWidget::OnGameOverClicked()
{
    // 1. GameDataInstance 가져오기
    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(this);
    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("GameDataInstance is null!"));
        return;
    }

    // 2. 메인 메뉴로 이동
    UGameplayStatics::OpenLevel(this, GameData->MainMenuLevelName);

    // 3. 위젯 제거 (자동으로 HUD 복구)
    RemoveFromParent();
}