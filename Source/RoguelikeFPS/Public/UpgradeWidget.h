#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidget.generated.h"

class UUpgradeSystem;
class UItemBase;
class UInventory;
class UInventoryWidget;
class UImage;
class UTextBlock;
class UButton;

UCLASS()
class ROGUELIKEFPS_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    // 초기화
    void InitWidget(UUpgradeSystem* InUpgradeSystem, UInventory* InInventory);
    void BindInventory(UInventoryWidget* InventoryWidget);

    UFUNCTION()
    void OnItemSelected(UItemBase* SelectedItem);
    
    UFUNCTION()
    void OnUpgradeClicked();

protected:
    UPROPERTY()
    UUpgradeSystem* UpgradeSystem;
    UPROPERTY()
    UInventory* Inventory;

    UPROPERTY(meta = (BindWidget))
    UImage* TargetItemImage;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TargetItemName;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TargetItemGrade;
    UPROPERTY(meta = (BindWidget))
    UImage* MaterialImage;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaterialName;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaterialCount;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* GoldCostText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SuccessRateText;
    UPROPERTY(meta = (BindWidget))
    UButton* UpgradeButton;

    //델리게이트
    UFUNCTION()
    void OnInventoryUpdated();
    UFUNCTION()
    void OnGoldChanged(int32 NewGold);
    UFUNCTION()
    void UpdateUI();

    UPROPERTY()
    UItemBase* SelectedPart;
    UPROPERTY()
    UItemBase* UpgradeMaterial;

    int32 GoldCost = 100;
};
