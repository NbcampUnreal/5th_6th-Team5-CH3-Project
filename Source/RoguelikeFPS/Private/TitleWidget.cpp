#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"
#include "FPSGameMode.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

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
    // GameMode에 이벤트 전달
    AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (FPSGameMode)
    {
        // GameMode의 OnTitleStartClicked 호출 (Title 제거 및 MainMenu 띄우기 담당)
        FPSGameMode->OnTitleStartClicked();
    }
    else
    {
        // GameMode를 찾을 수 없으면, 직접 MainMenu를 띄우는 로직 실행 (Fallback)
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
    UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}
