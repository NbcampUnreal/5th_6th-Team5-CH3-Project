#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

class APlayerController;
class UAugmentWidget;
class UTitleWidget;
class UMainMenuWidget;

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

    friend class UTitleWidget;
    friend class UMainMenuWidget;

public:
    AFPSGameMode(const FObjectInitializer& ObjectInitializer);

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void BeginPlay() override;

    //타이틀 & 메인메뉴
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTitleWidget> TitleWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    // 증강 선택 UI 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass;

protected:
    UPROPERTY()
    UTitleWidget* TitleWidgetInstance;

    UPROPERTY()
    UMainMenuWidget* MainMenuWidgetInstance;

    //타이틀 UI 이벤트
    UFUNCTION()
    void OnTitleStartClicked();

    UFUNCTION()
    void OnMainMenuStartClicked();

    UFUNCTION()
    void OnMainMenuBackClicked();

    //증강 이벤트
    UFUNCTION()
    void HandlePlayerLevelUp(APlayerController* PlayerController);

    UFUNCTION()
    void HandlePlayerDeath(AController* KillerController);

    UFUNCTION()
    void CloseCurrentUIAndResumeGame(bool bResumeGameInput = true);
};
