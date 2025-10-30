#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AugmentWidget.generated.h"

class UButton;
class UStatsComponent;

UCLASS()
class ROGUELIKEFPS_API UAugmentWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

public:
    // 증강 옵션 버튼들 (UMG에서 BindWidget으로 연결)
    UPROPERTY(meta = (BindWidget)) UButton* AugmentButton_1;
    UPROPERTY(meta = (BindWidget)) UButton* AugmentButton_2;
    UPROPERTY(meta = (BindWidget)) UButton* AugmentButton_3;

    // 증강 적용 후 게임 재개 및 위젯 제거를 위해 PlayerController 포인터 필요
    UPROPERTY(BlueprintReadOnly)
    APlayerController* OwningController = nullptr;

    // 이 위젯이 적용할 StatsComponent 포인터
    UPROPERTY(BlueprintReadOnly)
    UStatsComponent* TargetStatsComponent = nullptr;

protected:
    UFUNCTION()
    void OnAugment1Clicked() { HandleAugmentSelection(1); }
    UFUNCTION()
    void OnAugment2Clicked() { HandleAugmentSelection(2); }
    UFUNCTION()
    void OnAugment3Clicked() { HandleAugmentSelection(3); }

private:
    void HandleAugmentSelection(int32 SelectionIndex);
};
