#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AugmentWidget.h"
#include "FPSGameMode.generated.h"

class UTitleWidget;
class UMainMenuWidget;
class APlayerController;
class UAugmentWidget;

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()
    friend class UTitleWidget;
    friend class UMainMenuWidget;
    friend class UGameClearWidget;
public:
    AFPSGameMode(const FObjectInitializer& ObjectInitializer);
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void BeginPlay() override;

    // 타이틀 & 메인메뉴
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTitleWidget> TitleWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    // 증강 선택 UI 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    UDataTable* AugmentDataTable;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass;

    // 희귀도별 가중치 (블루프린트에서 설정 가능)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TMap<EAugmentRarity, float> RarityWeights;

    // 증강 이벤트
    UFUNCTION(BlueprintCallable, Category = "Augment")
    void HandlePlayerLevelUp(APlayerController* PlayerController);
    UFUNCTION()
    void CloseCurrentUIAndResumeGame(bool bResumeGameInput = true);

protected:
    UPROPERTY()
    UTitleWidget* TitleWidgetInstance;
    UPROPERTY()
    UMainMenuWidget* MainMenuWidgetInstance;

    // 타이틀 UI 이벤트
    UFUNCTION()
    void OnTitleStartClicked();
    UFUNCTION()
    void OnMainMenuStartClicked();
    UFUNCTION()
    void OnMainMenuBackClicked();
    UFUNCTION()
    void HandlePlayerDeath(AController* KillerController);
};