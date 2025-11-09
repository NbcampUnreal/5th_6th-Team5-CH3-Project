#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

class UButton;
class APlayerController;

#include "DeathWidget.generated.h"

//Death라고 했지만 사실 Clear도 포함
UCLASS()
class ROGUELIKEFPS_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnRestartClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnExitClicked();

    UPROPERTY(meta = (BindWidget))
    class UButton* RestartButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

protected:
    virtual bool Initialize() override;
};