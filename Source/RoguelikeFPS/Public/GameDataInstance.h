#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameDataInstance.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameDataInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "GameData")
    int32 SelectedWeaponIndex;

    UPROPERTY(BlueprintReadWrite, Category = "GameData")
    bool bIsReadyToStart;

    UFUNCTION(BlueprintCallable, Category = "GameData")
    void Init();

    UFUNCTION(BlueprintCallable, Category = "GameData")
    void ResetGameStatsToLevelOne();

    UFUNCTION(BlueprintCallable, Category = "GameData")
    void SetSelectedOption(int32 Index);
};
