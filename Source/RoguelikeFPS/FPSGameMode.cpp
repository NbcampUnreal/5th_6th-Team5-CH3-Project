#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "GameDataInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    DefaultPawnClass = AFPSCharacter::StaticClass();

    TitleWidgetClass = nullptr;
    MainMenuWidgetClass = nullptr;
    AugmentWidgetClass = nullptr;
}

// BeginPlay
void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Begin"));
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerController none"));
        return;
    }

    if (TitleWidgetClass)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Title"));
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Title Show"));
            TitleWidgetInstance->AddToViewport();
            PC->SetInputMode(FInputModeUIOnly());
            PC->bShowMouseCursor = true;

            UE_LOG(LogTemp, Warning, TEXT("Title Widget shown on Main Menu"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TitleWidgetClass not assigned in GameMode"));
    }
}

// Ingame
void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer || !NewPlayer->GetPawn()) return;

    // 레벨업 이벤트 구독
    if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
    {
        FPSChar->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
    }
}

// 타이틀 / 메인 메뉴 이벤트
void AFPSGameMode::OnTitleStartClicked()
{
    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(World, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();
            MainMenuWidgetInstance->OnBackButtonClicked.AddDynamic(this, &AFPSGameMode::OnMainMenuBackClicked);
            
            MainMenuWidgetInstance->Button_Start->OnClicked.AddDynamic(this, &AFPSGameMode::OnMainMenuBackClicked);
        }
    }
}
//무기 선택
void AFPSGameMode::OnMainMenuBackClicked()
{
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(World, TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AFPSGameMode::OnTitleStartClicked);
        }
    }
}

void AFPSGameMode::OnMainMenuStartClicked()
{
    if (!MainMenuWidgetInstance) return;
    {
        UGameDataInstance* GameInstance = Cast<UGameDataInstance>(GetWorld()->GetGameInstance());
        if (!GameInstance || !GameInstance->bIsReadyToStart)
        {
            UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Cannot start game: Weapon not selected!"), true, true, FColor::Red, 5.f);
            return;
        }
    }
    MainMenuWidgetInstance->RemoveFromParent();
    MainMenuWidgetInstance = nullptr;

    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Map1"));
}


// 레벨업 처리
void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController || !AugmentWidgetClass) return;

    PlayerController->SetPause(true);
    FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    UAugmentWidget* AugmentWidget = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);
    if (AugmentWidget)
    {
        AugmentWidget->AddToViewport();
        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Augment Selection Screen Opened."), true, true, FColor::Orange, 10.f);
    }
}

// 플레이어 사망 처리
void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GAME OVER!"), true, true, FColor::Red, 10.f);
    // 게임 일시정지
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetPause(true);
    }
    // 죽으면 메인 메뉴 씬으로
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_MainMenu"));
}
