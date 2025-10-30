#include "GameDataInstance.h"

void UGameDataInstance::SetSelectedOption(int32 OptionIndex)
{
    SelectedWeaponIndex = OptionIndex;

    bIsReadyToStart = (SelectedWeaponIndex != 0);
}

void UGameDataInstance::ResetGameStatsToLevelOne()
{
    // 게임 재시작 시 필요한 모든 상태를 초기 상태로 되돌립니다.
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;

    // (만약 XP, Level 등의 상태가 GameInstance에 있다면 여기서 초기화)
}
