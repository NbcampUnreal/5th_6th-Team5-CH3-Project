#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "FPSGameMode.h" // GameMode를 직접 호출할 경우 필요

bool UTitleWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);

    if (OptionButton)
        OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);

    return true;
}

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 혹시 모를 커서 활성화
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UTitleWidget::OnGameStartClicked()
{
    // GameMode를 통해 로직을 처리하도록 이벤트를 전달하는 것이 더 구조적입니다.
    AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (FPSGameMode)
    {
        // GameMode의 protected 함수를 호출하여 MainMenu 띄우기 및 입력 모드 전환 처리
        FPSGameMode->OnTitleStartClicked();
    }
    else
    {
        // GameMode를 찾을 수 없을 때의 Fallback (직접 MainMenu 띄우기)
        RemoveFromParent();
        if (MainMenuWidgetClass)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);

            if (MainMenu)
            {
                MainMenu->AddToViewport();
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                PC->SetInputMode(InputMode);
            }
        }
    }
}

void UTitleWidget::OnOptionClicked()
{
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu Placeholder"));
}

void UTitleWidget::OnExitClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    // 게임 종료 (실제 빌드에서만 작동)
    UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}