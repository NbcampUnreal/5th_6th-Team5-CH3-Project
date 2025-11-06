#include "StatsHUD.h"
#include "Blueprint/UserWidget.h"

void AStatsHUD::BeginPlay()
{
    Super::BeginPlay();

    if (StatsWidgetClass)
    {
        StatsWidget = CreateWidget<UUserWidget>(GetWorld(), StatsWidgetClass);
        if (StatsWidget)
        {
            StatsWidget->AddToViewport();
        }
    }
}
