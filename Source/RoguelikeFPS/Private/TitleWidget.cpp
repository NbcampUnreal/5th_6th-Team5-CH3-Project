#include "TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuWidget.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.h"
#include "FPSGameMode.h"
//for git commit
bool UTitleWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    // 버튼 이벤트 바인딩
    if (StartButton) StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnGameStartClicked);
    if (OptionButton) OptionButton->OnClicked.AddDynamic(this, &UTitleWidget::OnOptionClicked);
    if (ExitButton) ExitButton->OnClicked.AddDynamic(this, &UTitleWidget::OnExitClicked);
    return true;
}

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 입력 모드는 MainMenuController에서 관리
}

void UTitleWidget::OnGameStartClicked()
{
    // 메인 메뉴로 전환 (MainMenuController 위임)
    if (AMainMenuController* MC = Cast<AMainMenuController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        MC->ShowWeaponSelectMenu();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenuController not found, fallback to MainMenuWidget"));
        // 폴백: 메인 메뉴 직접 생성
        if (MainMenuWidgetClass)
        {
            UMainMenuWidget* MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
            if (MainMenuWidget)
            {
                MainMenuWidget->AddToViewport();
                APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
                if (PC)
                {
                    FInputModeUIOnly InputMode;
                    InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
                    PC->SetInputMode(InputMode);
                    PC->bShowMouseCursor = true;
                }
                RemoveFromParent();
            }
        }
    }
}

void UTitleWidget::OnOptionClicked()
{
    // 옵션 메뉴 플레이스홀더
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Option Menu Placeholder"));
}

void UTitleWidget::OnExitClicked()
{
    // 게임 종료
    UWorld* World = GetWorld();
    if (!World) return;
    UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World, 0), EQuitPreference::Quit, true);
}