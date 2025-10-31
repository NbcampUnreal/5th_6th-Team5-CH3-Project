// GameDataInstance.cpp
#include "GameDataInstance.h"
#include "GameFramework/HUD.h"
#include "StatsHUD.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UGameDataInstance::SetSelectedOption(int32 OptionIndex)
{
    // 1. 인덱스 제한 + 상태 업데이트
    SelectedWeaponIndex = FMath::Clamp(OptionIndex, 0, 4);
    bIsReadyToStart = (SelectedWeaponIndex != 0);

    // 2. HUD 즉시 갱신 (옵션, Tick에서도 감지하므로 생략 가능)
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (UUserWidget* Widget = Cast<UUserWidget>(PC->GetHUD()))
            {
                if (UStatsHUD* HUD = Cast<UStatsHUD>(Widget))
                {
                    HUD->ForceUpdateWeaponSelection();  // StatsHUD에 추가할 함수
                }
            }
        }
    }
}

void UGameDataInstance::ResetGameStatsToLevelOne()
{
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
}
