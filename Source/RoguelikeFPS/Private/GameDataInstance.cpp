#include "GameDataInstance.h"
#include "Kismet/GameplayStatics.h"
//for git commit
UGameDataInstance::UGameDataInstance()
{
    // 기본값 설정
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
    PlayerXP = 0.f;
    PlayerLevel = 1;
    XPToLevelUp = 100.f;
    CurrentStageIndex = 1;
    StageLevelNames.Add(FName("L_GameMap1"));
    StageLevelNames.Add(FName("L_GameMap2"));
    StageLevelNames.Add(FName("L_GameMap3"));
    MainMenuLevelName = FName("L_MainMenu");
}

UGameDataInstance* UGameDataInstance::GetGameDataInstance(UWorld* World)
{
    // GameInstance 캐스팅 유틸리티
    return Cast<UGameDataInstance>(UGameplayStatics::GetGameInstance(World));
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
    // 게임 데이터 초기화
    PlayerLevel = 1;
    PlayerXP = 0.f;
    SelectedWeaponIndex = 0;
    bIsReadyToStart = false;
    CurrentStageIndex = 1;
}