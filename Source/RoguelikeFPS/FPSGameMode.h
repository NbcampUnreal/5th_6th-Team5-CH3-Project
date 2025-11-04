#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

class APlayerController;
class UAugmentWidget;

UCLASS()
class ROGUELIKEFPS_API AFPSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AFPSGameMode(const FObjectInitializer& ObjectInitializer);

    virtual void PostLogin(APlayerController* NewPlayer) override;

    // 증강 선택 UI 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Augment")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass;

    UFUNCTION()
    void HandlePlayerLevelUp(APlayerController* PlayerController);

    UFUNCTION()
    void HandlePlayerDeath(AController* KillerController);
};
