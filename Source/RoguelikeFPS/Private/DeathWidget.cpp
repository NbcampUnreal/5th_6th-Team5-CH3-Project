// DeathWidget.cpp
#include "DeathWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameDataInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UDeathWidget::Setup(APlayerController* PC, bool bAnyKey)
{
    // 필요 시 사용 (현재는 비워둠)
    // 예: 타이머, 애니메이션 등
}

void UDeathWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GameOverButton2)
    {
        GameOverButton2->OnClicked.AddDynamic(this, &UDeathWidget::OnExitClicked);
    }
}

void UDeathWidget::OnExitClicked()
{
    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(this);
    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("GameDataInstance is null!"));
        return;
    }

    UGameplayStatics::OpenLevel(this, GameData->MainMenuLevelName);

    RemoveFromParent();
}