#include "AugmentWidget.h"
#include "UIManager.h"
#include "FPSCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"

bool UAugmentWidget::Initialize()
{
    if (!AugmentButton1 || !AugmentButton2 || !AugmentButton3)
    {
        UE_LOG(LogTemp, Error, TEXT("AugmentButton not bound in AugmentWidget"));
        return false;
    }
    AugmentButton1->OnClicked.AddDynamic(this, &UAugmentWidget::ApplyAugment1);
    AugmentButton2->OnClicked.AddDynamic(this, &UAugmentWidget::ApplyAugment2);
    AugmentButton3->OnClicked.AddDynamic(this, &UAugmentWidget::ApplyAugment3);
    return true;
}

void UAugmentWidget::Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments)
{
    OwningCharacter = InCharacter;
    Augments = InAugments;
}

void UAugmentWidget::ApplyAugment(int32 AugmentIndex)
{
    if (Augments.IsValidIndex(AugmentIndex) && OwningCharacter)
    {
        if (UAugmentSystem* AugmentSys = GetGameInstance()->GetSubsystem<UAugmentSystem>())
        {
            AugmentSys->ApplyAugment(OwningCharacter, Augments[AugmentIndex].AugmentID);
        }
    }
    if (UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>())
    {
        UIManager->HideWidget(this);
        UIManager->SetInputModeGame();
    }
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetPause(false);
    }
}