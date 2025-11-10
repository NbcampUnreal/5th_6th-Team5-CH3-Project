#include "UpgradeMenuWidget.h"
#include "InventoryWidget.h"
#include "UpgradeWidget.h"
#include "Inventory.h"
#include "UpgradeSystem.h"

void UUpgradeMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (InventoryPanel)
    {
        InventoryPanel->OnItemClicked.AddDynamic(this, &UUpgradeMenuWidget::OnItemSelected);
    }
}

void UUpgradeMenuWidget::InitMenu(UInventory* InInventory, UUpgradeSystem* InUpgradeSystem)
{
    Inventory = InInventory;
    UpgradeSystem = InUpgradeSystem;

    if (InventoryPanel && Inventory)
    {
        if (Inventory->InventoryUI && Inventory->InventoryUI != InventoryPanel)
        {
            Inventory->InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
        }

        InventoryPanel->InitInventory(Inventory);
        InventoryPanel->UpdateUI();
        Inventory->InventoryUI = InventoryPanel;
    }

    if (UpgradePanel && UpgradeSystem && Inventory)
    {
        UpgradePanel->InitWidget(UpgradeSystem, Inventory);
    }

    UE_LOG(LogTemp, Warning, TEXT("[UpgradeMenuWidget] InitMenu -> Inventory: %s | InventoryPanel: %s | UpgradePanel: %s"),
        *GetNameSafe(Inventory), *GetNameSafe(InventoryPanel), *GetNameSafe(UpgradePanel));
}

void UUpgradeMenuWidget::OnItemSelected(UItemBase* SelectedItem)
{
    if (UpgradePanel)
    {
        UpgradePanel->OnItemSelected(SelectedItem);
    }
    if (InventoryPanel)
    {
        InventoryPanel->UpdateUI();
    }
}