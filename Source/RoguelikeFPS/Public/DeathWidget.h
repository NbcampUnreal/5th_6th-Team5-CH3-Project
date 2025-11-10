// DeathWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DeathWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UDeathWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Setup 함수 추가 (FPSGameMode에서 호출)
    UFUNCTION(BlueprintCallable, Category = "UI")
    void Setup(APlayerController* PC, bool bAnyKey);

protected:
    virtual void NativeConstruct() override;
    UPROPERTY(meta = (BindWidget))
    UButton* GameOverButton2;

    UFUNCTION()
    void OnExitClicked();
};