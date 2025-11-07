#include "GameDataInstance.h"

UGameDataInstance::UGameDataInstance()
{
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
    PlayerXP = 0.f;
    PlayerLevel = 1;
    XPToLevelUp = 100.f;
}

void UGameDataInstance::SetSelectedOption(int32 WeaponIndex)
{
    SelectedWeaponIndex = WeaponIndex;
}

int32 UGameDataInstance::GetSelectedOption() const
{
    return SelectedWeaponIndex;
}

void UGameDataInstance::ResetGameStatsToLevelOne()
{
    PlayerLevel = 1;
    PlayerXP = 0.f;
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
}
