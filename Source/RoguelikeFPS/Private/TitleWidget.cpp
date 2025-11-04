#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"
#include "TimerManager.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UTitleWidget::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

    if (GameStartButton)
    {
        GameStartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);
    }

    if (OptionButton)
    {
        OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);
    }

    return true;
}

void UTitleWidget::OnGameStartClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    RemoveFromParent();
    if (UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), UMainMenuWidget::StaticClass()))
    {
        MainMenu->AddToViewport();
    }
}

void UTitleWidget::OnOptionClicked()
{
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu Placeholder"));
}

void UTitleWidget::OnExitClicked()
{
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
}
