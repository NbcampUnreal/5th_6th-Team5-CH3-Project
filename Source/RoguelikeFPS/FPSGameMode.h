#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AugmentWidget.h"
#include "FPSGameMode.generated.h"
//for git commit
class UTitleWidget;
class UMainMenuWidget;
class APlayerController;
class UAugmentWidget;
class UDeathWidget;

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()
    friend class UTitleWidget;
    friend class UMainMenuWidget;
    friend class UDeathWidget;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
    bool bIsReadyToStart = true;

    AFPSGameMode(const FObjectInitializer& ObjectInitializer);
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void BeginPlay() override;

    // UI 위젯 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTitleWidget> TitleWidgetClass; // 타이틀 UI 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass; // 메인 메뉴 UI 클래스
    UPROPERTY()
    TObjectPtr<UTitleWidget> TitleWidgetInstance; // 타이틀 인스턴스
    UPROPERTY()
    TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance; // 메인 메뉴 인스턴스
    UPROPERTY()
    TObjectPtr<UUserWidget> CurrentWidget; // 현재 활성 UI 위젯

    // 증강 데이터 및 설정
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TObjectPtr<UDataTable> AugmentDataTable; // 증강 데이터 테이블
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass; // 증강 UI 클래스
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Augment")
    TMap<EAugmentRarity, float> RarityWeights; // 희귀도별 가중치

    // 플레이어 및 UI 관리 함수
    UFUNCTION(BlueprintCallable, Category = "Augment")
    void HandlePlayerLevelUp(APlayerController* PlayerController);
    UFUNCTION()
    void CloseCurrentUIAndResumeGame(bool bResumeGameInput = true);

protected:
    UFUNCTION()
    void OnTitleStartClicked(); // 타이틀 시작
    UFUNCTION()
    void OnMainMenuStartClicked(); // 메인 메뉴 시작
    UFUNCTION()
    void OnMainMenuBackClicked(); // 메인 메뉴 뒤로
    UFUNCTION()
    void HandlePlayerDeath(AController* KillerController); // 사망 처리
};