#include "FPSGameMode.h"
#include "StatsComponent.h"
#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.h" 

AFPSGameMode::AFPSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // AugmentWidgetClass는 이제 헤더에서 설정되므로 주석 처리
}

void AFPSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // 1. 레벨업 이벤트와 사망 이벤트를 모두 구독
    if (NewPlayer && NewPlayer->GetPawn())
    {
        // UStatsComponent 구독 (레벨업)
        UStatsComponent* StatsComp = NewPlayer->GetPawn()->FindComponentByClass<UStatsComponent>();
        if (StatsComp)
        {
            StatsComp->OnLevelUp.AddDynamic(this, &AFPSGameMode::HandlePlayerLevelUp);
        }

        // AFPSCharacter 구독 (사망)
        AFPSCharacter* FPSChar = Cast<AFPSCharacter>(NewPlayer->GetPawn());
        if (FPSChar)
        {
            // C++ 델리게이트 바인딩: 사망 이벤트 발생 시 HandlePlayerDeath 호출
            FPSChar->OnPlayerDeath.AddDynamic(this, &AFPSGameMode::HandlePlayerDeath);
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

        // TargetStatsComponent 설정
        UStatsComponent* TargetStats = PlayerController->GetPawn()->FindComponentByClass<UStatsComponent>();
        AugmentWidget->TargetStatsComponent = TargetStats;

        AugmentWidget->AddToViewport();

        UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Augment Selection Screen Opened."), true, true, FColor::Orange, 10.0f);
    }
}

// 2. 사망 이벤트 핸들러 함수 추가
void AFPSGameMode::HandlePlayerDeath(AController* KillerController)
{
    // 킬러 컨트롤러는 사망자가 아닐 수 있으므로, 사망한 플레이어 컨트롤러 갖고옴
    APlayerController* DeadPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

    if (!DeadPlayerController) return;

    // Game Over UI를 띄우는 로직 (UI는 블루프린트에서 제작 필요)
    UKismetSystemLibrary::PrintString(GetWorld(), TEXT("GAME OVER! Returning to Main Menu."), true, true, FColor::Red, 10.0f);

    // 3. Game Over 후 메인 메뉴로 복귀 (게임 루프 종료 및 재시작)
    DeadPlayerController->SetPause(true); // 게임 일시정지 (선택 사항)

    // 메인 메뉴 레벨로 클라이언트 트래블 명령 (Game Over 상태를 클리어하고 복귀)
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_MainMenu"));

}