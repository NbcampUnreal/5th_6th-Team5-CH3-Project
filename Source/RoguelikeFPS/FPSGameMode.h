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
    // 1. 생성자 선언 추가
    AFPSGameMode(const FObjectInitializer& ObjectInitializer);

    // 2. PostLogin 함수 선언 추가
    virtual void PostLogin(APlayerController* NewPlayer) override;

    // 3. AugmentWidgetClass 변수 선언 추가
    UPROPERTY(EditDefaultsOnly, Category = "Augment")
    TSubclassOf<UAugmentWidget> AugmentWidgetClass;

    // UStatsComponent의 델리게이트와 연결될 함수
    UFUNCTION()
    void HandlePlayerLevelUp(APlayerController* PlayerController);
};
