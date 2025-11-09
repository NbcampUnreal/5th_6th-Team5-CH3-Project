#include "FPSGameMode.h"
#include "UIManager.h"
#include "AugmentSystem.h"
#include "AugmentWidget.h"
#include "FPSPlayerController.h"
#include "FPSCharacter.h"
#include "GameDataInstance.h"
#include "Inventory.h"
#include "Kismet/GameplayStatics.h"
#include <StatsHUD.h>

AFPSGameMode::AFPSGameMode()
{
    UE_LOG(LogTemp, Log, TEXT("AFPSGameMode Constructor Called"));
    CurrentGameState = EGameState::MainMenu;
}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();
    if (UGameDataInstance* GameData = Cast<UGameDataInstance>(GetGameInstance()))
    {
        UE_LOG(LogTemp, Log, TEXT("bGameStarted: %d"), GameData->bGameStarted); // 수정: 형식 문자열
        if (!GameData->bGameStarted)
        {
            SetGameState(EGameState::MainMenu);
            GameData->TeleportCount = 0;
            UE_LOG(LogTemp, Log, TEXT("FPSGameMode: Main"));
        }
        else
        {
            SetGameState(EGameState::Playing);
            UE_LOG(LogTemp, Log, TEXT("FPSGameMode: Play Setting"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameDataInstance null"));
        SetGameState(EGameState::MainMenu);
    }
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer || !NewPlayer->GetPawn()) return;

    if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
    {
        UE_LOG(LogTemp, Log, TEXT("HandlePlaterLevelUp"));
        FPSChar->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);

        if (UUIManager* UIMgr = GetGameInstance()->GetSubsystem<UUIManager>())
        {
            if (UUserWidget* HUDWidget = UIMgr->ShowWidget(StatsHUDClass, EUILayer::Background))
            {
                if (UStatsHUD* StatsHUD = Cast<UStatsHUD>(HUDWidget))
                {
                    StatsHUD->SetOwningCharacter(FPSChar);  // 핵심: 캐릭터 이벤트 바인딩
                }
            }
        }
    }
}

void AFPSGameMode::SetGameState(EGameState NewState)
{
    UUIManager* UIManagerRef = GetGameInstance()->GetSubsystem<UUIManager>();
    if (!UIManagerRef)
    {
        UE_LOG(LogTemp, Warning, TEXT("UIManager가 null입니다!"));
        return;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    switch (NewState)
    {
    case EGameState::MainMenu:
        UIManagerRef->ShowTitleScreen();
        UIManagerRef->SetInputModeUI();
        break;
    case EGameState::Playing:
        UIManagerRef->HideAllWidgetsExceptLayer(EUILayer::Background);
        UIManagerRef->ShowWidget(StatsHUDClass, EUILayer::Background);
        UIManagerRef->SetInputModeGame();
        if (PC) PC->SetPause(false);
        break;
    case EGameState::LevelUp:
        if (PC) HandlePlayerLevelUp(PC);
        break;
    case EGameState::GameOver:
        UIManagerRef->ShowWidget(UIManagerRef->DeathWidgetClass, EUILayer::Modal);
        UIManagerRef->SetInputModeUI();
        if (PC) PC->SetPause(true);
        break;
    case EGameState::GameClear:
        UIManagerRef->ShowWidget(UIManagerRef->GameClearWidgetClass, EUILayer::Modal);
        UIManagerRef->SetInputModeUI();
        if (PC) PC->SetPause(true);
        break;
    case EGameState::Paused:
        if (PC) PC->SetPause(true);
        break;
    }
}

void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController) return;
    PlayerController->SetPause(true);

    UUIManager* UIManagerRef = GetGameInstance()->GetSubsystem<UUIManager>();
    if (UIManagerRef)
    {
        if (UAugmentSystem* AugmentSys = GetGameInstance()->GetSubsystem<UAugmentSystem>())
        {
            TArray<FAugmentData> Augments = AugmentSys->GetRandomAugments(3);
            if (UIManagerRef->AugmentWidgetClass)
            {
                UAugmentWidget* AugmentWidget = Cast<UAugmentWidget>(
                    UIManagerRef->ShowWidget(UIManagerRef->AugmentWidgetClass, EUILayer::Modal));
                if (AugmentWidget)
                {
                    AFPSCharacter* Character = Cast<AFPSCharacter>(PlayerController->GetPawn());
                    if (Character)
                    {
                        AugmentWidget->Setup(Character, Augments);
                        UIManagerRef->SetInputModeUI();
                    }
                }
            }
        }
    }
}

void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    SetGameState(EGameState::GameOver);
}

