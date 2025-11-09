#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UGameDataInstance();

    // 플레이어 데이터
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 SelectedWeaponIndex; // 선택된 무기 인덱스
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    bool bIsReadyToStart; // 게임 시작 준비 여부
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float PlayerXP; // 플레이어 경험치
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 PlayerLevel; // 플레이어 레벨
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float XPToLevelUp; // 레벨업 필요 경험치

    // 스테이지 관리
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    int32 CurrentStageIndex; // 현재 스테이지 인덱스
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    TArray<FName> StageLevelNames; // 스테이지 맵 목록
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    FName MainMenuLevelName; // 메인 메뉴 레벨 이름

    // 유틸리티 함수
    UFUNCTION(BlueprintCallable, Category = "GameData")
    static UGameDataInstance* GetGameDataInstance(UWorld* World);
    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void SetSelectedOption(int32 WeaponIndex);
    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    int32 GetSelectedOption() const;
    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void ResetGameStatsToLevelOne();
};