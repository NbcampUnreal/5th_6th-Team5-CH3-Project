//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/PlayerController.h"
//#include "MainMenuController.generated.h"
//
//// 전방 선언 (헤더 경량화)
//class UTitleWidget;
//class UMainMenuWidget;
//
///**
// * 메인 메뉴 전용 PlayerController
// * - 타이틀 화면 → 무기 선택 메뉴 전환 관리
// * - UI 입력 모드 및 포커스 관리
// */
//UCLASS()
//class ROGUELIKEFPS_API AMainMenuController : public APlayerController
//{
//    GENERATED_BODY()
//
//public:
//    AMainMenuController() = default;
//
//protected:
//    virtual void BeginPlay() override;
//
//    // 타이틀 화면 위젯 클래스
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
//    TSubclassOf<UTitleWidget> TitleWidgetClass;
//
//    // 메인 메뉴(무기 선택) 위젯 클래스
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
//    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;
//
//    // 현재 활성화된 타이틀 위젯 인스턴스
//    UPROPERTY()
//    TObjectPtr<UTitleWidget> TitleWidgetInstance;
//
//    // 현재 활성화된 메인 메뉴 위젯 인스턴스
//    UPROPERTY()
//    TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;
//
//public:
//    //타이틀 화면을 표시
//    UFUNCTION()
//    void ShowTitleScreen();
//
//    // 무기 선택 화면을 표시
//    UFUNCTION()
//    void ShowWeaponSelectMenu();
//};
