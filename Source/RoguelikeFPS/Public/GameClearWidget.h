#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameClearWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UGameClearWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 버튼 (GameOverButton 하나만!)
    UPROPERTY(meta = (BindWidget))
    UButton* GameOverButton;

    // 클릭 이벤트
    UFUNCTION()
    void OnGameOverClicked();
};