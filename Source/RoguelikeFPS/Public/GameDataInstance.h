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

    // ===== 플레이어 선택/상태 데이터 =====
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 SelectedWeaponIndex;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    bool bIsReadyToStart;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float PlayerXP;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    int32 PlayerLevel;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
    float XPToLevelUp;

    // ===== 맵 순환 관리 (스테이지 진행 추적) =====
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    int32 CurrentStageIndex; // 현재 진행 중인 스테이지 인덱스 (1부터 시작)

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    TArray<FName> StageLevelNames; // 맵 이름 목록 (에디터에서 설정)

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameFlow")
    FName MainMenuLevelName; // 메인 메뉴 레벨 이름

public:
    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void SetSelectedOption(int32 WeaponIndex);

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    int32 GetSelectedOption() const;

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void ResetGameStatsToLevelOne();
};