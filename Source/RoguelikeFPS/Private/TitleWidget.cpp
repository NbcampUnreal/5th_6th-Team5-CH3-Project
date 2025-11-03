#include "TitleWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"
#include "Engine/Engine.h"

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
    OnStartButtonClicked.Broadcast();
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
