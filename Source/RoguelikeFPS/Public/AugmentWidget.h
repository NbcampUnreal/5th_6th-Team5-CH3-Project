// AugmentWidget.h

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

public:
    // **[수정]** Initialize()는 일반적으로 public에 선언합니다.
    virtual bool Initialize() override;

public:
    // 증강 옵션 버튼들 (UMG에서 BindWidget으로 연결)
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton_1;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton_2;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> AugmentButton_3;

    // **[수정]** TObjectPtr 적용
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<APlayerController> OwningController = nullptr;

    // **[수정]** TObjectPtr 적용
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UStatsComponent> TargetStatsComponent = nullptr;

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