#include "GameDataInstance.h"

UGameDataInstance::UGameDataInstance()
{
    // ===== 기본값 설정 =====
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
    PlayerXP = 0.f;
    PlayerLevel = 1;
    XPToLevelUp = 100.f;

    // 맵 순환 관리 초기화
    CurrentStageIndex = 1; // 첫 번째 맵부터 시작

    // 기본 맵 목록 (에디터에서 수정 가능)
    StageLevelNames.Add(FName("L_GameMap1"));
    StageLevelNames.Add(FName("L_GameMap2"));
    StageLevelNames.Add(FName("L_GameMap3"));

    MainMenuLevelName = FName("L_MainMenu");
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
    CurrentStageIndex = 1;  //게임 오버/클리어 후 재시작을 위해 CurrentStageIndex 리셋
}