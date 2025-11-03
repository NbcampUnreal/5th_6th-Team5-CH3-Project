#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StatsHUD.generated.h"

UCLASS()
class ROGUELIKEFPS_API AStatsHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> StatsWidgetClass;

    UPROPERTY()
    class UUserWidget* StatsWidget;
};