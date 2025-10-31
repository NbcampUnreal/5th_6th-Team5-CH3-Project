// MainMenuController.cpp

#include "MainMenuController.h"
#include "TitleWidget.h"
#include "MainMenuWidget.h" 
#include "Blueprint/UserWidget.h" 

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	// 디버그 메시지 (WITH_EDITOR로 감싸는 것을 권장)
	#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MainMenuController BeginPlay Called! Starting with TitleWidget."));
	#endif

    // **[수정]** TitleWidget 생성 및 표시 로직
	if (TitleWidgetClass) // TitleWidgetClass는 TSubclassOf<UTitleWidget>로 가정
	{
		// 위젯 생성 및 포인터에 저장
		TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);

		if (TitleWidgetInstance)
		{
			TitleWidgetInstance->AddToViewport();

			// TitleWidget의 Start 버튼 델리게이트에 전환 함수 바인딩
			TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);

			// UI 전용 입력 모드 설정 (TitleWidget에 포커스)
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
			SetInputMode(InputModeData);

			// 마우스 커서 표시
			bShowMouseCursor = true;

			return; // TitleWidget을 성공적으로 표시했으므로 함수 종료
		}
	}
    
    // **[개선]** Fallback: TitleWidgetClass가 설정되지 않았거나 생성에 실패했을 경우의 처리
    // (이 경우 Input이 잠기지 않게 Game and UI 모드로 설정)
    FInputModeGameAndUI InputModeData; // 게임 중에도 UI 조작 가능 모드 (TitleWidget이 없으니 마우스 잠금을 해제)
	SetInputMode(InputModeData);
	bShowMouseCursor = true;
}

void AMainMenuController::ShowWeaponSelectMenu()
{
    // 1. 현재 타이틀 위젯 제거
    if (TitleWidgetInstance)
    {
        TitleWidgetInstance->RemoveFromParent();
        TitleWidgetInstance = nullptr;
    }

    // 2. 무기 선택 위젯 (MainMenuWidget) 생성 및 표시
    if (MainMenuWidgetClass)
    {
        // 위젯 생성 및 포인터에 저장
        MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);

        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            MainMenuWidgetInstance->OnBackButtonClicked.AddDynamic(this, &AMainMenuController::ShowTitleScreen);

            // 새로운 위젯에 포커스 설정
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
            SetInputMode(InputModeData);
            // bShowMouseCursor는 이미 true입니다.
        }
    }
}

void AMainMenuController::ShowTitleScreen()
{
    // 1. 현재 메인 메뉴 위젯 제거
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }
    if (TitleWidgetClass)
    {
        TitleWidgetInstance = CreateWidget<UTitleWidget>(this, TitleWidgetClass);

        if (TitleWidgetInstance)
        {
            TitleWidgetInstance->AddToViewport();

            // Start 버튼에 다시 ShowWeaponSelectMenu 연결
            TitleWidgetInstance->OnStartButtonClicked.AddDynamic(this, &AMainMenuController::ShowWeaponSelectMenu);

            // 포커스 설정
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(TitleWidgetInstance->TakeWidget());
            SetInputMode(InputModeData);
        }
    }
}