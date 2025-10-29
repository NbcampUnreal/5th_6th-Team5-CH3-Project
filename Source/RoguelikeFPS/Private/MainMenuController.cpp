#include "MainMenuController.h"
#include "Blueprint/UserWidget.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	// 디버그 메시지
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MainMenuController BeginPlay Called!"));

	if (MainMenuWidgetClass)
	{
		// 위젯 생성 및 포인터에 저장
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);

		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
		}
	}

	// UI 전용 입력 모드 설정
	FInputModeUIOnly InputModeData;
	if (MainMenuWidgetInstance)
	{
		InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
	}
	SetInputMode(InputModeData);

	// 마우스 커서 표시
	bShowMouseCursor = true;
}