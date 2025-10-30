#include "FPSGameMode.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // ... 기존 생성자 코드 ...
    // AugmentWidgetClass를 여기서 DefaultSubobject로 설정하거나, BP 자식 클래스에서 할당해야 함.
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // 플레이어 캐릭터가 스폰된 후 StatsComponent의 이벤트를 구독합니다.
    if (NewPlayer && NewPlayer->GetPawn())
    {
        UStatsComponent* StatsComp = NewPlayer->GetPawn()->FindComponentByClass<UStatsComponent>();
        if (StatsComp)
        {
            // C++ 델리게이트 바인딩: 레벨업 이벤트 발생 시 HandlePlayerLevelUp 호출
            StatsComp->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        }
    }
}
void AFPSGameMode::HandlePlayerLevelUp(APlayerController* PlayerController)
{
    if (!PlayerController || !AugmentWidgetClass) return;

    // 1. GameMode가 게임을 일시정지하고 입력 모드를 변경 (서버/권한)
    PlayerController->SetPause(true);

    FInputModeUIOnly InputMode;
    PlayerController->SetInputMode(InputMode);

    // 2. 증강 위젯 생성 및 표시 (클라이언트에 명령)
    UAugmentWidget* AugmentWidget = CreateWidget<UAugmentWidget>(PlayerController, AugmentWidgetClass);

    if (AugmentWidget)
    {
        // 위젯에 필요한 정보 전달
        AugmentWidget->OwningController = PlayerController;
        AugmentWidget->TargetStatsComponent = PlayerController->GetPawn()->FindComponentByClass<UStatsComponent>();

        AugmentWidget->AddToViewport();

        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Augment Selection Screen Opened."), true, true, FColor::Orange, 10.0f);
    }
}