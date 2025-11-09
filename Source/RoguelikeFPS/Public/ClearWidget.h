#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ClearWidget.generated.h"

class UButton;
class UGameDataInstance;
class UUIManager;

UCLASS()
class ROGUELIKEFPS_API UClearWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* RestartButton;
    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;
    UPROPERTY()
    class UGameDataInstance* GameDataInstance;
    UPROPERTY()
    class UUIManager* UIManager;
    UFUNCTION()
    void OnRestartClicked();
    UFUNCTION()
    void OnExitClicked();
};