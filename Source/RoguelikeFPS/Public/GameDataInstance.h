#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "Game")
    bool TitleMenu = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameFlow")
    TArray<FName> StageLevelNames;
    UPROPERTY(BlueprintReadWrite, Category = "GameFlow")
    int32 CurrentStageIndex = 1;
    UPROPERTY(BlueprintReadWrite, Category = "GameFlow")
    int32 SelectedWeaponIndex;
    UFUNCTION(BlueprintCallable, Category = "GameFlow")
    void ResetGameStatsToLevelOne();
    UPROPERTY(BlueprintReadWrite, Category = "Game Data")
    bool IsWeaponSelected = false;
    UPROPERTY(BlueprintReadWrite, Category = "Game")
        bool bGameStarted = false;
    UPROPERTY(BlueprintReadWrite, Category = "GameFlow")
        int32 TeleportCount = 0;
    UFUNCTION(BlueprintCallable, Category = "GameFlow")
    void IncrementTeleportCount();
    UFUNCTION(BlueprintCallable, Category = "GameFlow")
    bool ShouldTriggerGameClear() const;
};