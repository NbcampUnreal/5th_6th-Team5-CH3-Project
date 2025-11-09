#include "GameDataInstance.h"
void UGameDataInstance::ResetGameStatsToLevelOne()
{
    CurrentStageIndex = 0; // 변경: 첫 스테이지부터 시작
    SelectedWeaponIndex = 0;
    IsWeaponSelected = false;
    bGameStarted = false; // 추가: 선언 후 사용
    TeleportCount = 0; // 추가: 텔레포트 횟수 초기화
}

void UGameDataInstance::IncrementTeleportCount()
{
    TeleportCount++;
    UE_LOG(LogTemp, Log, TEXT("텔레포트 횟수: %d"), TeleportCount);
}

bool UGameDataInstance::ShouldTriggerGameClear() const
{
    return TeleportCount >= 3;
}