#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartSystem.h"
#include "ItemBase.h"
#include "PartWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class ROGUELIKEFPS_API UPartWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void InitWidget(UPartSystem* InPartSystem, UInventory* InInventory);

    UFUNCTION()
    void UpdateUI();

    // 슬롯별 버튼
    UFUNCTION()
    void OnMuzzleEquipClicked();
    UFUNCTION()
    void OnMuzzleUnequipClicked();

    UFUNCTION()
    void OnMagazinEquipClicked();
    UFUNCTION()
    void OnMagazinUnequipClicked();

    UFUNCTION()
    void OnStockEquipClicked();
    UFUNCTION()
    void OnStockUnequipClicked();

protected:
    // 연결된 시스템
    UPROPERTY()
    UPartSystem* PartSystem;

    UPROPERTY()
    UInventory* LinkedInventory;

    // 머즐
    UPROPERTY(meta = (BindWidget))
    UImage* MuzzleImage;
    UPROPERTY(meta = (BindWidget))
    UButton* MuzzleEquipButton;
    UPROPERTY(meta = (BindWidget))
    UButton* MuzzleUnequipButton;

    // 탄창
    UPROPERTY(meta = (BindWidget))
    UImage* MagazinImage;
    UPROPERTY(meta = (BindWidget))
    UButton* MagazinEquipButton;
    UPROPERTY(meta = (BindWidget))
    UButton* MagazinUnequipButton;

    // 스톡
    UPROPERTY(meta = (BindWidget))
    UImage* StockImage;
    UPROPERTY(meta = (BindWidget))
    UButton* StockEquipButton;
    UPROPERTY(meta = (BindWidget))
    UButton* StockUnequipButton;

    // 상태 출력
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PartStatusText;
};
