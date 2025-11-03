#include "GameDataInstance.h"

void UGameDataInstance::Init()
{
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
}

void UGameDataInstance::ResetGameStatsToLevelOne()
{
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
}

void UGameDataInstance::SetSelectedOption(int32 Index)
{
    SelectedWeaponIndex = Index;
}
