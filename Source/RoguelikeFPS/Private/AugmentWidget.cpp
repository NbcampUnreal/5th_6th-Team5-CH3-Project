#include "AugmentWidget.h"
#include "Components/Button.h"
#include "StatsComponent.h"
#include "GameFramework/PlayerController.h"

bool UAugmentWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    // 버튼 이벤트 바인딩
    if (AugmentButton_1) { AugmentButton_1->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment1Clicked); }
    if (AugmentButton_2) { AugmentButton_2->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment2Clicked); }
    if (AugmentButton_3) { AugmentButton_3->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment3Clicked); }

    return true;
}

void UAugmentWidget::HandleAugmentSelection(int32 SelectionIndex)
{
    if (!TargetStatsComponent) return;
    if (!OwningController) return;

    // 1. 증강 적용 로직 호출
    TargetStatsComponent->ApplyAugment(SelectionIndex);

    // 2. 증강 UI 제거
    RemoveFromParent();

    // 3. 게임 일시정지 해제 및 입력 모드 복구
    OwningController->SetPause(false);

    // 게임 재개 후 일반적인 게임 입력 모드로 복구 (예: UI Only -> Game Only)
    FInputModeGameOnly InputMode;
    OwningController->SetInputMode(InputMode);
}
