#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "FPSGameMode.h"
//for git commit
bool UAugmentWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (AugmentButton1) AugmentButton1->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment1Clicked);
    if (AugmentButton2) AugmentButton2->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment2Clicked);
    if (AugmentButton3) AugmentButton3->OnClicked.AddDynamic(this, &UAugmentWidget::OnAugment3Clicked);

    return true;
}

void UAugmentWidget::Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments)
{
    OwningCharacter = InCharacter;
    AvailableAugments = InAugments;

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
}

void UAugmentWidget::OnAugment1Clicked() { ApplyAugment(0); }
void UAugmentWidget::OnAugment2Clicked() { ApplyAugment(1); }
void UAugmentWidget::OnAugment3Clicked() { ApplyAugment(2); }

void UAugmentWidget::ApplyAugment(int32 AugmentIndex)
{
    if (!OwningCharacter.IsValid() || AugmentIndex >= AvailableAugments.Num()) return;

    const FAugmentData& SelectedAugment = AvailableAugments[AugmentIndex];
    OwningCharacter->ApplyAugment(SelectedAugment.AugmentID);

    // Notify GameMode to close UI and resume game
    if (AFPSGameMode* GameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        GameMode->CloseCurrentUIAndResumeGame(true);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Applied Augment: %s"), *SelectedAugment.DisplayName.ToString()));
    }

    RemoveFromParent();
}