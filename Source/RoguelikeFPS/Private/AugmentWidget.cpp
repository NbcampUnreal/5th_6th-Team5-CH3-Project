#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "FPSGameMode.h"
bool UAugmentWidget::Initialize()
{
    if (!Super::Initialize()) return false;
    if (AugmentButton1) AugmentButton1->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment1Clicked);
    if (AugmentButton2) AugmentButton2->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment2Clicked);
    if (AugmentButton3) AugmentButton3->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment3Clicked);
    if (ConfirmButton)  ConfirmButton->OnClicked.AddDynamic(this, &UAugmentWidget::OnConfirmClicked);
    //확정 버튼 비활성화
    UpdateConfirmButton();
    if (GetWorld())
    {
        UpdateConfirmButton();
    }
    return true;
}
void UAugmentWidget::Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments)
{
    OwningCharacter = InCharacter;
    AvailableAugments = InAugments;
    SelectedAugmentIndex = -1;
    if (AvailableAugments.Num() >= 1 && AugmentName1 && AugmentDesc1)
    {
        AugmentName1->SetText(AvailableAugments[0].DisplayName);
        AugmentDesc1->SetText(AvailableAugments[0].Description);
    }
    if (AvailableAugments.Num() >= 2 && AugmentName2 && AugmentDesc2)
    {
        AugmentName2->SetText(AvailableAugments[1].DisplayName);
        AugmentDesc2->SetText(AvailableAugments[1].Description);
    }
    if (AvailableAugments.Num() >= 3 && AugmentName3 && AugmentDesc3)
    {
        AugmentName3->SetText(AvailableAugments[2].DisplayName);
        AugmentDesc3->SetText(AvailableAugments[2].Description);
    }
    UpdateConfirmButton();
}
void UAugmentWidget::SelectAugment(int32 Index)
{
    if (Index >= AvailableAugments.Num()) return;
    SelectedAugmentIndex = Index;
    UpdateConfirmButton();
    UE_LOG(LogTemp, Warning, TEXT("Augment %d Selected"), Index);
}
void UAugmentWidget::OnConfirmClicked()
{
    ApplyAugment(SelectedAugmentIndex);
}
void UAugmentWidget::ApplyAugment(int32 Index)
{
    if (!OwningCharacter.IsValid() || Index < 0 || Index >= AvailableAugments.Num()) return;
    const FAugmentData& Augment = AvailableAugments[Index];
    OwningCharacter->ApplyAugment(Augment.AugmentID);
    // 세이브: 선택한 증강 기록 (옵션)
    if (GameInstance)
    {
        // 예: GameInstance->LastAppliedAugment = Augment.AugmentID;
        GameInstance->SaveGameData();
    }
    // GameMode에 UI 종료 요청
    if (AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->CloseCurrentUIAndResumeGame(true);
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("Applied Augment: %s"), *Augment.DisplayName.ToString()));
    }
    RemoveFromParent();
}
void UAugmentWidget::UpdateConfirmButton()
{
    if (ConfirmButton)
    {
        ConfirmButton->SetIsEnabled(SelectedAugmentIndex != -1);
    }
}