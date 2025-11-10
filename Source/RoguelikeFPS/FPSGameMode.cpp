// FPSGameMode.cpp
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsReadyToStart = true;

    static ConstructorHelpers::FObjectFinder<UDataTable> AugmentDataTableFinder(TEXT("/Game/Data/DT_Augments.DT_Augments"));
    if (AugmentDataTableFinder.Succeeded())
    {
        AugmentDataTable = AugmentDataTableFinder.Object;
    }

    RarityWeights.Add(EAugmentRarity::Normal, 0.50f);
    RarityWeights.Add(EAugmentRarity::Rare, 0.30f);
    RarityWeights.Add(EAugmentRarity::Epic, 0.15f);
    RarityWeights.Add(EAugmentRarity::Legendary, 0.05f);


}

void AFPSGameMode::BeginPlay()
{
    Super::BeginPlay();

    FString FullMapPath = GetWorld()->GetMapName();
    FString CleanName = FPaths::GetCleanFilename(FullMapPath);
    FString PackageName = GetWorld()->GetOutermost()->GetName();

    UE_LOG(LogTemp, Warning, TEXT("=== [TitleWidget Debug] ==="));
    UE_LOG(LogTemp, Warning, TEXT("Full Map Path: %s"), *FullMapPath);
    UE_LOG(LogTemp, Warning, TEXT("Clean Name: %s"), *CleanName);

    if (!FullMapPath.Contains(TEXT("MainMenu")) &&
        !CleanName.Contains(TEXT("MainMenu")) &&
        !PackageName.Contains(TEXT("MainMenu")))
    {
        UE_LOG(LogTemp, Warning, TEXT("MainMenu not detected → Skip TitleWidget"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("MainMenu DETECTED! → Creating TitleWidget..."));

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
            {
                if (APlayerController* DelayedPC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
                {
                    CreateTitleWidget(DelayedPC);
                }
            }, 0.1f, false);
        return;
    }

    CreateTitleWidget(PC);
}

void AFPSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //UWorld* w = GetWorld();
    //ACharacter* C = UGameplayStatics::GetPlayerCharacter(w, 0);
    //AFPSCharacter* Character = nullptr;
    //if(C)
    //{
    //    Character = Cast<AFPSCharacter>(C);
    //}
    //
    //UGameDataInstance* instance = Cast<UGameDataInstance>(GetGameInstance());

    //if(!Character)UE_LOG(LogTemp, Log, TEXT("Character is NULL"));
    //if(!instance)UE_LOG(LogTemp, Log, TEXT("instance is NULL"));

    //instance->SaveStatus(Character);


    Super::EndPlay(EndPlayReason);
}

void AFPSGameMode::CreateTitleWidget(APlayerController* PC)
{
    if (!PC || !TitleWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("CreateTitleWidget: PC or TitleWidgetClass is null!"));
        return;
    }

    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }

    TitleWidgetInstance = CreateWidget<UTitleWidget>(PC, TitleWidgetClass);
    if (!TitleWidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to CreateWidget<UTitleWidget>!"));
        return;
    }

    TitleWidgetInstance->AddToViewport(1000);
    PC->bShowMouseCursor = true;
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    HideStatsHUD();
    UE_LOG(LogTemp, Warning, TEXT("TitleWidget FULLY CREATED!"));
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    if (!NewPlayer || !NewPlayer->GetPawn()) return;
    if (AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
    UE_LOG(LogTemp, Log, TEXT("Player logged in: %s"), *NewPlayer->GetName());
    if (StatsHUDClass && !StatsHUDInstance)
    {
        StatsHUDInstance = CreateWidget<UStatsHUD>(NewPlayer, StatsHUDClass);
        if (StatsHUDInstance)
        {
            StatsHUDInstance->AddToViewport();
            if (AFPSCharacter* Char = Cast<AFPSCharacter>(NewPlayer->GetPawn()))
            {
                StatsHUDInstance->SetOwningCharacter(Char);
            }
        }
    }
    ShowStatsHUD();

    FInputModeGameOnly GameInput;
    NewPlayer->SetInputMode(GameInput);
    NewPlayer->bShowMouseCursor = false;
}

void AFPSGameMode::HideStatsHUD(bool bHideMouse)
{
    if (StatsHUDInstance)
    {
        StatsHUDInstance->SetVisibility(ESlateVisibility::Hidden);
    }
    if (bHideMouse)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
            PC->bShowMouseCursor = false;
    }
}

void AFPSGameMode::ShowStatsHUD()
{
    if (StatsHUDInstance)
    {
        StatsHUDInstance->SetVisibility(ESlateVisibility::Visible);
    }
}

void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController || !AugmentWidgetClass) return;

    AugmentWidgetInstance = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);
    if (AugmentWidgetInstance)
    {
        AugmentWidgetInstance->AddToViewport(1000);
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(AugmentWidgetInstance->TakeWidget());
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
        if (UStatsHUD* HUD = GetHUDInstance(PlayerController))
        {
            HUD->HideHUD();
        }
    }
}

void AFPSGameMode::CloseCurrentUIAndResumeGame(bool bResumeGameInput)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    if (TitleWidgetInstance) { TitleWidgetInstance->RemoveFromParent(); TitleWidgetInstance = nullptr; }
    if (MainMenuWidgetInstance) { MainMenuWidgetInstance->RemoveFromParent(); MainMenuWidgetInstance = nullptr; }
    if (DeathWidgetInstance) { DeathWidgetInstance->RemoveFromParent(); DeathWidgetInstance = nullptr; }
    if (AugmentWidgetInstance) { AugmentWidgetInstance->RemoveFromParent(); AugmentWidgetInstance = nullptr; }
    if (GameClearWidgetInstance) { GameClearWidgetInstance->RemoveFromParent(); GameClearWidgetInstance = nullptr; }

    if (bResumeGameInput)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
        if (UStatsHUD* HUD = GetHUDInstance(PC))
        {
            HUD->ShowHUD();
        }
    }
}

void AFPSGameMode::OnTitleStartClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnTitleStartClicked: Title → MainMenu"));

    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(GetWorld()->GetFirstPlayerController(), MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport(1000);
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
                PC->SetInputMode(InputMode);
                HideStatsHUD();
            }
            // 델리게이트 바인딩 제거 - MainMenuWidget에서 직접 호출
        }
    }
}

void AFPSGameMode::OnMainMenuStartClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnMainMenuStartClicked: Start Game!"));

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(GetWorld());
    if (!GameData || GameData->StageLevelNames.Num() == 0) return;

    FName NextLevel = GameData->StageLevelNames[0];
    UE_LOG(LogTemp, Warning, TEXT("Loading first stage: %s"), *NextLevel.ToString());
    UGameplayStatics::OpenLevel(GetWorld(), NextLevel);
}

void AFPSGameMode::OnMainMenuBackClicked()
{
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    UGameDataInstance* GameData = UGameDataInstance::GetGameDataInstance(GetWorld());
    if (!GameData) return;

    UGameplayStatics::OpenLevel(GetWorld(), GameData->MainMenuLevelName);
}

void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && DeathWidgetClass)
    {
        DeathWidgetInstance = CreateWidget<UDeathWidget>(PC, DeathWidgetClass);
        if (DeathWidgetInstance)
        {
            DeathWidgetInstance->Setup(PC, false);
            DeathWidgetInstance->AddToViewport(1000);
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(DeathWidgetInstance->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            HideStatsHUD();
        }
    }
}

void AFPSGameMode::HandleGameClear()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC && GameClearWidgetClass)
    {
        GameClearWidgetInstance = CreateWidget<UGameClearWidget>(PC, GameClearWidgetClass);
        if (GameClearWidgetInstance)
        {
            GameClearWidgetInstance->AddToViewport(1000);
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(GameClearWidgetInstance->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
            HideStatsHUD();
        }
    }
}

UStatsHUD* AFPSGameMode::GetHUDInstance(APlayerController* PC)
{
    if (!PC || !StatsHUDInstance) return nullptr;
    return (StatsHUDInstance->GetOwningPlayer() == PC) ? StatsHUDInstance : nullptr;
}

void AFPSGameMode::InternalHideStatsHUD(APlayerController* PC)
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UStatsHUD::StaticClass(), true);
    for (UUserWidget* Widget : Widgets)
    {
        Widget->SetVisibility(ESlateVisibility::Hidden);
        return;
    }
}

void AFPSGameMode::InternalShowStatsHUD(APlayerController* PC)
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UStatsHUD::StaticClass(), true);
    for (UUserWidget* Widget : Widgets)
    {
        Widget->SetVisibility(ESlateVisibility::Visible);
        return;
    }
}

