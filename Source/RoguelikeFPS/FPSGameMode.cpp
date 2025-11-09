#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "GameDataInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UDataTable> AugmentDataTableFinder(TEXT("/Game/Data/DT_Augments.DT_Augments"));
    if (AugmentDataTableFinder.Succeeded())
    {
        AugmentDataTable = AugmentDataTableFinder.Object;
    }
    // 기본 가중치 설정
    RarityWeights.Add(EAugmentRarity::Normal, 0.50f);
    RarityWeights.Add(EAugmentRarity::Rare, 0.30f);
    RarityWeights.Add(EAugmentRarity::Epic, 0.15f);
    RarityWeights.Add(EAugmentRarity::Legendary, 0.05f);
}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();
            if (MainMenuWidgetClass)
            {
                TitleWidgetInstance->MainMenuWidgetClass = MainMenuWidgetClass;
            }
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer || !NewPlayer->GetPawn()) return;
    if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
    {
        FPSChar->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
    }
}

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
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}

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
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}

void AFPSGameMode::OnMainMenuStartClicked()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    FName NextLevelName = TEXT("L_GameMap01");
    if (UGameDataInstance* GameDataInstance = Cast<UGameDataInstance>(GameInstance))
    {
        if (GameDataInstance->CurrentStageIndex >= 1 &&
            GameDataInstance->CurrentStageIndex <= GameDataInstance->StageLevelNames.Num())
        {
            NextLevelName = GameDataInstance->StageLevelNames[GameDataInstance->CurrentStageIndex - 1];
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                FString::Printf(TEXT("Opening Level: %s (Stage Index: %d)"), *NextLevelName.ToString(), GameDataInstance->CurrentStageIndex));
            GameDataInstance->CurrentStageIndex++;
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
                FString::Printf(TEXT("Error: CurrentStageIndex %d out of bounds for StageLevelNames array size %d. Using fallback."),
                    GameDataInstance->CurrentStageIndex, GameDataInstance->StageLevelNames.Num()));
        }
    }
    UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
}

void AFPSGameMode::CloseCurrentUIAndResumeGame(bool bResumeGameInput)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        for (TObjectIterator<UUserWidget> It; It; ++It)
        {
            if (It->IsInViewport())
            {
                It->RemoveFromParent();
            }
        }
        if (bResumeGameInput)
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }
        else
        {
            FInputModeUIOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
        PC->SetPause(false);
    }
}

void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("HandlePlayerLevelUp: PlayerController is NULL"));
    }
    if (!AugmentWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("HandlePlayerLevelUp: AugmentWidgetClass is NULL"));
    }
    if (!AugmentDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("HandlePlayerLevelUp: AugmentDataTable is NULL"));
    }

    if (!PlayerController || !AugmentWidgetClass || !AugmentDataTable) return;

    PlayerController->SetPause(true);
    FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);
    PlayerController->bShowMouseCursor = true;

    TArray<FAugmentData*> AllAugments;
    AugmentDataTable->GetAllRows(TEXT(""), AllAugments);
    TArray<FAugmentData> SelectedAugments;

    if (AllAugments.Num() > 0)
    {
        for (int32 i = 0; i < 3 && AllAugments.Num() > 0; ++i)
        {
            float TotalWeight = 0.0f;
            TArray<FAugmentData*> WeightedAugments;

            for (FAugmentData* Augment : AllAugments)
            {
                float Weight = RarityWeights.FindRef(Augment->Rarity);
                TotalWeight += Weight;
                for (int32 j = 0; j < Weight * 100; ++j)
                {
                    WeightedAugments.Add(Augment);
                }
            }

            int32 RandomIndex = FMath::RandRange(0, WeightedAugments.Num() - 1);
            FAugmentData* Selected = WeightedAugments[RandomIndex];
            SelectedAugments.Add(*Selected);
            AllAugments.Remove(Selected);
        }
    }

    UAugmentWidget* AugmentWidget = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("LEVELUP"));
    if (AugmentWidget)
    {
        AFPSCharacter* Character = Cast<AFPSCharacter>(PlayerController->GetPawn());
        if (Character)
        {
            AugmentWidget->Setup(Character, SelectedAugments);
            AugmentWidget->AddToViewport();
            UKismetSystemLibrary::PrintString(GetWorld(), TEXT("증강 선택 화면이 열렸습니다."), true, true, FColor::Orange, 10.f);
        }
    }
}

void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GAME OVER!"), true, true, FColor::Red, 10.f);
    FName MenuLevelName = TEXT("L_MainMenu");
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    if (UGameDataInstance* GameDataInstance = Cast<UGameDataInstance>(GameInstance))
    {
        MenuLevelName = GameDataInstance->MainMenuLevelName;
        GameDataInstance->ResetGameStatsToLevelOne();
        UE_LOG(LogTemp, Log, TEXT("GameDataInstance 리셋: MainMenuLevelName = %s"), *MenuLevelName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameDataInstance 캐스트 실패! Fallback 레벨: %s"), *MenuLevelName.ToString());
    }
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->SetPause(true);
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }
    FString OutReason;
    if (FPackageName::DoesPackageExist(MenuLevelName.ToString(), nullptr, &OutReason))
    {
        UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("레벨 '%s' 존재하지 않음! Fallback 사용."), *MenuLevelName.ToString());
        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("L_MainMenu")));  // 최종 Fallback
    }
    UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
}