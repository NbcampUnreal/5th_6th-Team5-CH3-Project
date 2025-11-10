#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeleportCountdownWidget.generated.h"

class UTextBlock;

UCLASS()
class ROGUELIKEFPS_API UTeleportCountdownWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void StartCountdown(float Duration);

protected:
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    float CountdownTime = 0.f;
    bool bIsCounting = false;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CountdownText;
};
