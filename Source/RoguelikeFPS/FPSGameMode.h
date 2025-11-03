#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFPSGameMode();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Level")
    void LoadLevel(FName LevelName);

    UFUNCTION()
    void HandlePlayerLevelUp();

    UFUNCTION()
    void HandlePlayerDeath();

    UFUNCTION()
    void HandleStageClear();
};
