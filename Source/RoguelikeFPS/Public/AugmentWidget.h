#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSCharacter.h"
#include "AugmentWidget.generated.h"

UCLASS()
class ROGUELIKEFPS_API UAugmentWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SelectAugment(FName AugmentName);

protected:
    // 캐릭터 포인터는 UPROPERTY로 GC 안전성 확보
    UPROPERTY()
    AFPSCharacter* PlayerCharacter = nullptr;
};
