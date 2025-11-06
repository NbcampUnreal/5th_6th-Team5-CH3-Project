#include "GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h" 
#include "Blueprint/UserWidget.h" // GetWorld, RemoveFromParent 사용을 위해 추가

bool UGameClearWidget::Initialize()
{
    if (!Super::Initialize())
        return false;

    // 함수 이름 통일: OnExitButtonClicked
    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnExitButtonClicked);

    return true; // *** 반드시 true로 반환 ***
}

void UGameClearWidget::SetupGameModeLink(AFPSGameMode* OwningGameMode)
{
    OwningGameModePtr = OwningGameMode;
}


void UGameClearWidget::OnExitButtonClicked() // 함수 이름 통일
{
    AFPSGameMode* GM = OwningGameModePtr.Get();
    // UUserWidget의 멤버 함수인 GetWorld() 사용
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (GM && PC)
    {
        // GameMode의 protected 함수 호출 (AFPSGameMode.h에 friend 선언 필수)
        //GM->ReturnToMainMenu(PC);

        RemoveFromParent(); // UUserWidget의 멤버 함수
    }
}