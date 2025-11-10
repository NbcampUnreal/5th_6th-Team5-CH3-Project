#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSCharacter.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h"
#include "DeathWidget.h"
#include "AugmentWidget.h"
#include "GameClearWidget.h"
#include "StatsHUD.h"
#include "GameDataInstance.h"
#include "FPSGameMode.generated.h"

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFPSGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // UI 이벤트
    UFUNCTION(BlueprintCallable) void OnTitleStartClicked();
    UFUNCTION(BlueprintCallable) void OnMainMenuStartClicked();
    UFUNCTION(BlueprintCallable) void OnMainMenuBackClicked();
    UFUNCTION(BlueprintCallable) void HandlePlayerLevelUp(APlayerController* PlayerController);
    UFUNCTION(BlueprintCallable) void PickRandomAugments(int32 Count, TArray<FAugmentData>& Out);
    UFUNCTION(BlueprintCallable) void CloseCurrentUIAndResumeGame(bool bResumeGameInput = true);
    UFUNCTION(BlueprintCallable) void HandlePlayerDeath(AController* KillerController);
    UFUNCTION(BlueprintCallable) void HandleGameClear();
    UFUNCTION(BlueprintCallable) void HideStatsHUD(bool bHideMouse = true);
    UFUNCTION(BlueprintCallable) void ShowStatsHUD();

protected:
    void InternalHideStatsHUD(APlayerController* PC);
    void InternalShowStatsHUD(APlayerController* PC);
    UPROPERTY() TObjectPtr<UStatsHUD> StatsHUDInstance;
    UStatsHUD* GetHUDInstance(APlayerController* PC);
    void CreateTitleWidget(APlayerController* PC);

    // 레벨업 시 증강 선택 팝업
    void ShowAugmentSelection(AFPSCharacter* Character);

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UTitleWidget> TitleWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UDeathWidget> DeathWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameClearWidget> GameClearWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UStatsHUD> StatsHUDClass;

    UPROPERTY() TObjectPtr<UTitleWidget> TitleWidgetInstance;
    UPROPERTY() TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;
    UPROPERTY() TObjectPtr<UDeathWidget> DeathWidgetInstance;
    UPROPERTY() TObjectPtr<UAugmentWidget> AugmentWidgetInstance;
    UPROPERTY() TObjectPtr<UGameClearWidget> GameClearWidgetInstance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Augment")
    TObjectPtr<UDataTable> AugmentDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Augment")
    TMap<EAugmentRarity, float> RarityWeights;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
    bool bIsReadyToStart = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    TArray<FName> StageLevelNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    FName MainMenuLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    int32 CurrentStageIndex = 0;
};