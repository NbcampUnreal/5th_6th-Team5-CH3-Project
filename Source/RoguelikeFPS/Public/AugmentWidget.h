#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentSystem.h"
#include "AugmentWidget.generated.h"

class AFPSCharacter;

UCLASS()
class ROGUELIKEFPS_API UAugmentWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Augment")
    void Setup(AFPSCharacter* InCharacter, const TArray<FAugmentData>& InAugments);

    UFUNCTION(BlueprintCallable, Category = "Augment")
    void ApplyAugment(int32 AugmentIndex);

    UPROPERTY(meta = (BindWidget))
    class UButton* AugmentButton1;

    UPROPERTY(meta = (BindWidget))
    class UButton* AugmentButton2;

    UPROPERTY(meta = (BindWidget))
    class UButton* AugmentButton3;

protected:
    virtual bool Initialize() override;

private:
    UPROPERTY()
    AFPSCharacter* OwningCharacter;

    UPROPERTY()
    TArray<FAugmentData> Augments;

    UFUNCTION()
    void ApplyAugment1() { ApplyAugment(0); }

    UFUNCTION()
    void ApplyAugment2() { ApplyAugment(1); }

    UFUNCTION()
    void ApplyAugment3() { ApplyAugment(2); }
};