#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

bool UTitleWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (GameStartButton)
        GameStartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);

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
    UWorld* World = GetWorld();
    if (!World) return;

    // 현재 타이틀 위젯 제거
    RemoveFromParent();

    // MainMenuWidgetClass가 지정되어 있다면 생성
    if (MainMenuWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
        UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass);

        if (MainMenu)
        {
            MainMenu->AddToViewport();
            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MainMenuWidgetClass not set in TitleWidget"));
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
