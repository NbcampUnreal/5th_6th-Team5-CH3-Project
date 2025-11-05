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
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 현재 아무것도 초기화하지 않아도 괜찮음.
}

// BeginPlay
void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();

            // Title 위젯에 MainMenu 클래스 설정 (Blueprint에서 설정 안 했을 경우 대비)
            if (MainMenuWidgetClass)
            {
                TitleWidgetInstance->MainMenuWidgetClass = MainMenuWidgetClass;
            }

            // 마우스 모드 세팅
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}

// Ingame
void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
//    Super::PostLogin(NewPlayer);
//
//    if (!NewPlayer || !NewPlayer->GetPawn()) return;
//
//    // 레벨업 이벤트 구독
//    if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
//    {
//        FPSChar->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
//        FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
//    }
}
//
//// 타이틀 / 메인 메뉴 이벤트
void AFPSGameMode::OnTitleStartClicked()
{
    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            // 입력 모드 재설정 (MainMenu가 포커스를 가짐)
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}
////무기 선택
void AFPSGameMode::OnMainMenuBackClicked()
{
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            // Title 위젯 이벤트 구독
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AFPSGameMode::OnTitleStartClicked);

            // 입력 모드 재설정 (Title 화면으로 복귀)
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}
//
void AFPSGameMode::OnMainMenuStartClicked()
{
    // 게임 레벨로 전환 시 입력 모드를 게임으로 돌려놔야 함 (중요!)
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
    const FName NextLevelName = TEXT("L_Map1");
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
        FString::Printf(TEXT("Opening Level: %s"), *NextLevelName.ToString()));

    UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
}

//UI닫기 및 게임 재개
void AFPSGameMode::CloseCurrentUIAndResumeGame(bool bResumeGameInput)
{
    // 모든 Viewport 위젯 제거 (강력한 방법)
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        TArray<UUserWidget*> Widgets;
        // Viewport에 있는 모든 위젯을 가져옵니다.
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            // 모든 Viewport 위젯을 순회하며 제거
            for (TObjectIterator<UUserWidget> It; It; ++It)
            {
                if (It->IsInViewport())
                {
                    It->RemoveFromParent();
                }
            }
        }

        // 입력 모드 복구
        if (bResumeGameInput)
        {
            // 게임 플레이 모드로 전환
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }
        else
        {
            // UI 모드 유지 (예: MainMenu에서 게임으로 돌아가지 않을 때)
            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
        PC->SetPause(false);
    }
}

// 레벨업 처리
void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController || !AugmentWidgetClass) return;

    PlayerController->SetPause(true);

    // UI 모드로 전환
    FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);
    PlayerController->bShowMouseCursor = true; // 커서 보이기

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
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    // 죽으면 메인 메뉴 씬으로
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_MainMenu"));
}
