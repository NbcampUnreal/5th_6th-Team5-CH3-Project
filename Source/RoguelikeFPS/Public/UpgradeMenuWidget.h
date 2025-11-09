#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeMenuWidget.generated.h"

class UInventoryWidget;
class UUpgradeWidget;
class UItemBase;

UCLASS()
class ROGUELIKEFPS_API UUpgradeMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void InitMenu(class UInventory* InInventory, class UUpgradeSystem* InUpgradeSystem);

protected:
    UPROPERTY(meta = (BindWidget))
    UInventoryWidget* InventoryPanel;

    UPROPERTY(meta = (BindWidget))
    UUpgradeWidget* UpgradePanel;

    UFUNCTION()
    void OnItemSelected(UItemBase* SelectedItem);

    UPROPERTY()
    class UInventory* Inventory;

    UPROPERTY()
    class UUpgradeSystem* UpgradeSystem;
};