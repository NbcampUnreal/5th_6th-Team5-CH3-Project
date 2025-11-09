#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "GameDataInstance.h"
#include "DeathWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.h"
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 증강 데이터 테이블 로드
    static ConstructorHelpers::FObjectFinder<UDataTable> AugmentDataTableFinder(TEXT("/Game/Data/DT_Augments.DT_Augments"));
    if (AugmentDataTableFinder.Succeeded())
    {
        AugmentDataTable = AugmentDataTableFinder.Object;
    }
    ensureAlwaysMsgf(AugmentDataTable, TEXT("AugmentDataTable not found!"));
    RarityWeights.Add(EAugmentRarity::Normal, 0.50f);
    RarityWeights.Add(EAugmentRarity::Rare, 0.30f);
    RarityWeights.Add(EAugmentRarity::Epic, 0.15f);
    RarityWeights.Add(EAugmentRarity::Legendary, 0.05f);
}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();
    // 타이틀 UI 표시
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            CurrentWidget = TitleWidgetInstance; // 현재 위젯 설정
            if (AMainMenuController* MC = Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController()))
            {
                MC->SetUIMode(TitleWidgetInstance.Get());
            }
        }
    }
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    // 플레이어 로그인 후 초기화
    UE_LOG(LogTemp, Log, TEXT("Player logged in: %s"), *NewPlayer->GetName());
}

void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    // 플레이어 레벨업 처리
    if (PlayerController && AugmentWidgetClass)
    {
        UAugmentWidget* AugmentWidget = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);
        if (AugmentWidget)
        {
            AugmentWidget->AddToViewport();
            CurrentWidget = AugmentWidget; // 현재 위젯 업데이트
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(AugmentWidget->TakeWidget());
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }
    }
}

void AFPSGameMode::CloseCurrentUIAndResumeGame(bool bResumeGameInput)
{
    // 현재 UI 닫고 게임 재개
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (CurrentWidget)
        {
            CurrentWidget->RemoveFromParent();
            CurrentWidget = nullptr; // 위젯 초기화
        }
        if (bResumeGameInput)
        {
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = false;
        }
    }
}

void AFPSGameMode::OnTitleStartClicked()
{
    // 타이틀 화면에서 시작 버튼 클릭
    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
        CurrentWidget = nullptr;
    }
if (MainMenuWidgetClass)
{
    MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->AddToViewport();
        CurrentWidget = MainMenuWidgetInstance; // 현재 위젯 설정
        if (AMainMenuController* MC = Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController()))
        {
            MC->SetUIMode(MainMenuWidgetInstance.Get());
        }
    }
}
}
void AFPSGameMode::OnMainMenuStartClicked()
{
    // 메인 메뉴에서 게임 시작
    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(GetWorld());
    if (!GameData) return;

    FName NextLevelName = TEXT("L_GameMap01");
    if (GameData->CurrentStageIndex >= 1 && GameData->CurrentStageIndex <= GameData->StageLevelNames.Num())
    {
        NextLevelName = GameData->StageLevelNames[GameData->CurrentStageIndex - 1];
        GameData->CurrentStageIndex++;
        UE_LOG(LogTemp, Log, TEXT("Opening Level: %s (Stage Index: %d)"), *NextLevelName.ToString(), GameData->CurrentStageIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Stage Index. Fallback to Main Menu: %s"), *GameData->MainMenuLevelName.ToString());
        NextLevelName = GameData->MainMenuLevelName;
    }
    UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
}

void AFPSGameMode::OnMainMenuBackClicked()
{
    // 메인 메뉴에서 뒤로 가기
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
        CurrentWidget = nullptr;
    }
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            CurrentWidget = TitleWidgetInstance; // 현재 위젯 설정
            if (AMainMenuController* MC = Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController()))
            {
                MC->SetUIMode(TitleWidgetInstance.Get());
            }
        }
    }
}

void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    // 플레이어 사망 처리
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        UDeathWidget* DeathWidget = CreateWidget<UDeathWidget>(PC, UDeathWidget::StaticClass());
        if (DeathWidget)
        {
            DeathWidget->Setup(PC, false); // 사망 상태로 설정
            DeathWidget->AddToViewport();
            CurrentWidget = DeathWidget; // 현재 위젯 설정
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(DeathWidget->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}