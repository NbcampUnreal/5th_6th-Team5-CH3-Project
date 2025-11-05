#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // 플레이어 선택/상태 데이터
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

public:
    UGameDataInstance();

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void SetSelectedOption(int32 WeaponIndex);

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    int32 GetSelectedOption() const;

    UFUNCTION(BlueprintCallable, Category = "PlayerData")
    void ResetGameStatsToLevelOne();
};
