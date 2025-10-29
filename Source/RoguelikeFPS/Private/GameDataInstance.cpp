#include "GameDataInstance.h"

void UGameDataInstance::SetSelectedOption(int32 OptionIndex)
{
	SelectedWeaponIndex = OptionIndex;

	bIsReadyToStart = (SelectedWeaponIndex != 0);
}
