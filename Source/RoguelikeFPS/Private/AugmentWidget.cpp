#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameDataInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "FPSGameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
bool UAugmentWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    // 버튼 바인딩 (예: UMG에서 OnClicked 연결)
    // UMG에서 연결했다면 생략 가능
    if (AugmentButton1) AugmentButton1->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment1Clicked);
    if (AugmentButton2) AugmentButton2->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment2Clicked);
    if (AugmentButton3) AugmentButton3->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment3Clicked);
    
    if (ConfirmButton)   ConfirmButton->OnClicked.AddDynamic(this, &UAugmentWidget::OnConfirmClicked);
    if (CloseButton)    CloseButton->OnClicked.AddDynamic(this, &UAugmentWidget::Close);


    return true;
}
void UAugmentWidget::Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments)
{
    OwningCharacter = InCharacter;
    AvailableAugments = InAugments;
    SelectedIndex = -1;
    // UI 초기화 (텍스트, 이미지 등)
    // 예: Augment1_Text->SetText(FText::FromString(Augments[0].Name));
    AugmentName1->SetText(FText::FromString(*AvailableAugments[0].DisplayName.ToString()));
    AugmentName2->SetText(FText::FromString(*AvailableAugments[1].DisplayName.ToString()));
    AugmentName3->SetText(FText::FromString(AvailableAugments[2].DisplayName.ToString()));

    AugmentDesc1->SetText(FText::FromString(*AvailableAugments[0].Description.ToString()));
    AugmentDesc2->SetText(FText::FromString(*AvailableAugments[1].Description.ToString()));
    AugmentDesc3->SetText(FText::FromString(AvailableAugments[2].Description.ToString()));
}
void UAugmentWidget::OnConfirmClicked()
{
    if (SelectedIndex < 0 || SelectedIndex >= AvailableAugments.Num()) return;
    if (!OwningCharacter.IsValid()) return;

    const FAugmentData& Augment = AvailableAugments[SelectedIndex];
    UE_LOG(LogTemp, Log, TEXT("TEXT : %s"), *Augment.AugmentID.ToString());
    OwningCharacter->ApplyAugment(Augment.AugmentID);



    // 2) 위젯 제거
    RemoveFromParent();
    // 1) 입력/마우스/커서 복구
    Close();
}
void UAugmentWidget::SelectAugment(int32 Index)
{
    if (Index < 0 || Index >= AvailableAugments.Num()) return;
    SelectedIndex = Index;
    // UI 하이라이트 업데이트
    // 예: Highlight_Augment1->SetVisibility(Index == 0 ? ESlateVisibility::Visible : Hidden);
}



void UAugmentWidget::Close()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        // 1) 게임 입력 복구 + 커서 숨김
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());

        if (AFPSCharacter* Player = Cast<AFPSCharacter>(PC->GetPawn()))
        {
            Player->EnableInput(PC);
        }

        // 2) StatsHUD 다시 표시 (레벨업 때 숨겼다면)
        if (UWorld* World = GetWorld())
        {
            if (AFPSGameMode* GM = World->GetAuthGameMode<AFPSGameMode>())
            {
                GM->ShowStatsHUD();
            }
        }
    }

    // 3) 이 위젯만 제거
    RemoveFromParent();
}
