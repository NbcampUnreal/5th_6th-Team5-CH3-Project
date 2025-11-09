#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    MainMenu,
    Playing,
    LevelUp,
    GameOver,
    GameClear,
    Paused
};

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AFPSGameMode();
    virtual void PostLogin(APlayerController* NewPlayer) override;
    UFUNCTION(BlueprintCallable, Category = "GameState")
    void SetGameState(EGameState NewState);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> StatsHUDClass;
    UFUNCTION()
    void HandlePlayerLevelUp(APlayerController* PlayerController);
protected:
    virtual void BeginPlay() override; // 수정: 중복 제거, protected에 유지
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState")
    EGameState CurrentGameState;
    UFUNCTION()
    void HandlePlayerDeath(AController* KillerController);
};