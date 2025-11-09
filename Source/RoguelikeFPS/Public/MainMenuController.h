#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

class UTitleWidget;
class UMainMenuWidget;

UCLASS()
class ROGUELIKEFPS_API AMainMenuController : public APlayerController
{
    GENERATED_BODY()
public:
    AMainMenuController() = default;

protected:
    virtual void BeginPlay() override;

    // UI 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UTitleWidget> TitleWidgetClass; // 타이틀 UI 클래스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass; // 메인 메뉴 UI 클래스

    // UI 인스턴스
    UPROPERTY()
    TObjectPtr<UTitleWidget> TitleWidgetInstance; // 타이틀 인스턴스
    UPROPERTY()
    TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance; // 메인 메뉴 인스턴스

public:
    // UI 표시 함수
    UFUNCTION()
    void ShowTitleScreen();
    UFUNCTION()
    void ShowWeaponSelectMenu();
    // 입력 모드 설정
    UFUNCTION()
    void SetUIMode(UUserWidget* Widget);
};