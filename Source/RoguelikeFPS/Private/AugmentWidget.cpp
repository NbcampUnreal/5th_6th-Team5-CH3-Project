#include "AugmentWidget.h"
#include "Kismet/GameplayStatics.h"

void UAugmentWidget::SelectAugment(FName AugmentName)
{
    if (!PlayerCharacter)
    {
        UWorld* World = GetWorld();
        if (!World) return;

        ACharacter* Char = UGameplayStatics::GetPlayerCharacter(World, 0);
        PlayerCharacter = Cast<AFPSCharacter>(Char);
    }

    if (PlayerCharacter)
    {
        PlayerCharacter->ApplyAugment(AugmentName);
    }
}
