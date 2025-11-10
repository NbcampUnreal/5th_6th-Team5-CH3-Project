#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h"
#include "Kismet/KismetSystemLibrary.h"

bool UTitleWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    if (StartButton) StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);
    if (OptionButton) OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);
    return true;
}

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void UTitleWidget::OnGameStartClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("TitleWidget: Start Clicked ¡æ GameMode.OnTitleStartClicked()"));

    if (AFPSGameMode* GM = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GM->OnTitleStartClicked();
    }
}

void UTitleWidget::OnOptionClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu"));
}

void UTitleWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, true);
}