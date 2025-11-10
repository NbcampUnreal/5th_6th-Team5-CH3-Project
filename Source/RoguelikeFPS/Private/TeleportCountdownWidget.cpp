#include "TeleportCountdownWidget.h"
#include "Components/TextBlock.h"
//for git commit
void UTeleportCountdownWidget::StartCountdown(float Duration)
{
    CountdownTime = Duration;
    bIsCounting = true;

    if (CountdownText)
    {
        CountdownText->SetText(FText::AsNumber(FMath::CeilToInt(CountdownTime)));
    }
}

void UTeleportCountdownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsCounting) return;

    CountdownTime -= InDeltaTime;
    if (CountdownText)
    {
        CountdownText->SetText(FText::AsNumber(FMath::Max(0, FMath::CeilToInt(CountdownTime))));
    }

    if (CountdownTime <= 0.f)
    {
        bIsCounting = false;
        RemoveFromParent();
    }
}
